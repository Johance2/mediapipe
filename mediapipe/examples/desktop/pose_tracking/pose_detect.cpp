//pose_detect.cpp
#include "pose_detect.h"
using namespace POSE_SAMPLE;
POSEResult pose_detect::init_graph(const char* model_path) noexcept {
	absl::Status run_status = InitGraph(model_path);
	if (!run_status.ok()) {
		return POSE_RESULT_GRAPH_INIT_EXCEPTION;
	}
	m_init = true;
    m_count = 33;
    m_pose_infos = new POSEInfo[m_count];
	return  POSE_RESULT_OK;
}

POSEResult pose_detect::detect_video(const char* video_path, int show_image, PoseFrameCallBack call) {
	if (!m_init)
		return POSE_RESULT_NOT_INIT;

	absl::Status run_status = RunMPPGraphVideo(video_path, show_image, call);
	if (!run_status.ok()) {
		return POSE_RESULT_RUN_GRAPH_EXCEPTION;
	}
	return POSE_RESULT_OK;
}

POSEResult pose_detect::detect_video2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count) {
	if (!m_init)
		return POSE_RESULT_NOT_INIT;

	absl::Status run_status = RunMPPGraphVideo2(rows, cols, data, infos, count);
	if (!run_status.ok()) {
		return POSE_RESULT_RUN_GRAPH_EXCEPTION;
	}
	return POSE_RESULT_OK;
}

POSEResult pose_detect::release() {
	absl::Status run_status = ReleaseGraph();
	if (!run_status.ok()) {
		return POSE_RESULT_RELEASE_EXCEPTION;
	}
    if(m_pose_infos)
    {
        delete [] m_pose_infos;
    }
	return POSE_RESULT_OK;
}

absl::Status pose_detect::InitGraph(const char* model_path) {
	std::string calculator_graph_config_contents;
	MP_RETURN_IF_ERROR(mediapipe::file::GetContents(model_path, &calculator_graph_config_contents));
	mediapipe::CalculatorGraphConfig config =
	    mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
	        calculator_graph_config_contents);

	MP_RETURN_IF_ERROR(m_graph.Initialize(config));

	auto sop = m_graph.AddOutputStreamPoller(kOutputStream);
	assert(sop.ok());
	p_poller = std::make_unique<mediapipe::OutputStreamPoller>(std::move(sop.value()));

	mediapipe::StatusOrPoller sop_landmark = m_graph.AddOutputStreamPoller(kOutputLandmarks);
	assert(sop_landmark.ok());
	p_poller_landmarks = std::make_unique<mediapipe::OutputStreamPoller>(std::move(sop_landmark.value()));

	MP_RETURN_IF_ERROR(m_graph.StartRun({}));

	return absl::OkStatus();
}

absl::Status pose_detect::RunMPPGraphVideo(const char* video_path, int show_image, PoseFrameCallBack call) {
	cv::VideoCapture capture;
	capture.open(video_path);
	RET_CHECK(capture.isOpened());

	if (show_image) {
		cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
	}
#if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
	capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	capture.set(cv::CAP_PROP_FPS, 30);
#endif

	bool grab_frames = true;
	while (grab_frames) {
		// Capture opencv camera or video frame.
		cv::Mat camera_frame_raw;
		capture >> camera_frame_raw;
		if (camera_frame_raw.empty())
			break;
            
		cv::Mat camera_frame;
		cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
		cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

		// Wrap Mat into an ImageFrame.
		auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
		                       mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
		                       mediapipe::ImageFrame::kDefaultAlignmentBoundary);
		cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
		camera_frame.copyTo(input_frame_mat);

		// Send image packet into the graph.
		size_t frame_timestamp_us =
		    (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;

		MP_RETURN_IF_ERROR(m_graph.AddPacketToInputStream(
		                       kInputStream, mediapipe::Adopt(input_frame.release())
		                       .At(mediapipe::Timestamp(frame_timestamp_us))));

		// Get the graph result packet, or stop if that fails.
		mediapipe::Packet packet;
		mediapipe::Packet packet_landmarks;
		if (!p_poller->Next(&packet)) break;
		if (p_poller_landmarks->QueueSize() > 0) {
			if (p_poller_landmarks->Next(&packet_landmarks)) {
				auto& output_landmarks = packet_landmarks.Get<mediapipe::NormalizedLandmarkList>();

                if(m_count != output_landmarks.landmark_size())
                {
                    m_count = output_landmarks.landmark_size();
                    if(m_pose_infos)
                    {
                        delete [] m_pose_infos;
                    }
                    m_pose_infos = new POSEInfo[m_count];
                }
				for (int i = 0; i < output_landmarks.landmark_size(); ++i)
				{
					const mediapipe::NormalizedLandmark landmark = output_landmarks.landmark(i);
					m_pose_infos[i].x = landmark.x() * camera_frame.cols;
					m_pose_infos[i].y = landmark.y() * camera_frame.rows;
                    m_pose_infos[i].v = landmark.visibility();
                    m_pose_infos[i].p = landmark.presence();
				}
				if (call) {
					call(m_pose_infos, m_count);
				}
			}
		}
		if (show_image) {
			auto& output_frame = packet.Get<mediapipe::ImageFrame>();
			// Convert back to opencv for display or saving.
			cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
			cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);
			cv::Mat dst;
			cv::resize(output_frame_mat, dst, cv::Size(output_frame_mat.cols / 2, output_frame_mat.rows / 2));
			cv::imshow(kWindowName, dst);
			cv::waitKey(1);
		}
	}
	if (show_image)
		cv::destroyWindow(kWindowName);
	return absl::OkStatus();
}


absl::Status pose_detect::RunMPPGraphVideo2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count) {

    do
    {

        cv::Mat camera_frame(rows, cols, CV_8UC3, data, cv::Mat::AUTO_STEP);
        // cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
        // cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

        // Wrap Mat into an ImageFrame.
        auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
                                mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
                                mediapipe::ImageFrame::kDefaultAlignmentBoundary);
        cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
        camera_frame.copyTo(input_frame_mat);

        // Send image packet into the graph.
        size_t frame_timestamp_us =
            (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;

        MP_RETURN_IF_ERROR(m_graph.AddPacketToInputStream(
                                kInputStream, mediapipe::Adopt(input_frame.release())
                                .At(mediapipe::Timestamp(frame_timestamp_us))));

        // Get the graph result packet, or stop if that fails.
        mediapipe::Packet packet;
        mediapipe::Packet packet_landmarks;
        if (!p_poller->Next(&packet)) break;
        if (p_poller_landmarks->QueueSize() > 0) {
            if (p_poller_landmarks->Next(&packet_landmarks)) {
                auto& output_landmarks = packet_landmarks.Get<mediapipe::NormalizedLandmarkList>();

                if(m_count != output_landmarks.landmark_size())
                {
                    m_count = output_landmarks.landmark_size();
                    if(m_pose_infos)
                    {
                        delete [] m_pose_infos;
                    }
                    m_pose_infos = new POSEInfo[m_count];
                }

                count = output_landmarks.landmark_size();
                for (int i = 0; i < output_landmarks.landmark_size() && i < count; ++i)
                {
                    const mediapipe::NormalizedLandmark landmark = output_landmarks.landmark(i);
                    m_pose_infos[i].x = landmark.x() * camera_frame.cols;
                    m_pose_infos[i].y = landmark.y() * camera_frame.rows;
                    m_pose_infos[i].v = landmark.visibility();
                    m_pose_infos[i].p = landmark.presence();
                }
                *infos = m_pose_infos;
                count = m_count;
            }
        }
    } while (0);
    return absl::OkStatus();
}

absl::Status pose_detect::ReleaseGraph() {
	MP_RETURN_IF_ERROR(m_graph.CloseInputStream(kInputStream));
	MP_RETURN_IF_ERROR(m_graph.CloseInputStream(kOutputLandmarks));
	return m_graph.WaitUntilDone();
}
