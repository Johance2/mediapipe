// pose_detect.h 实现 pose tracking
#ifndef POSE_DETECT_H
#define POSE_DETECT_H

#include <cstdlib>
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "pose_data.h"

namespace POSE_SAMPLE {
	typedef void(*PoseFrameCallBack)(POSEInfo* infos, int count);
	class pose_detect
	{
	public:
		POSEResult init_graph(const char* model_path) noexcept;
		POSEResult detect_video(const char* video_path, int show_image, PoseFrameCallBack call);
		POSEResult detect_video2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count);
		POSEResult release();
	private:
		absl::Status InitGraph(const char* model_path);
		absl::Status RunMPPGraphVideo(const char* video_path, int show_image, PoseFrameCallBack call);
		absl::Status RunMPPGraphVideo2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count);
		absl::Status ReleaseGraph();

		const char* kInputStream = "input_video";
		const char* kOutputStream = "output_video";
		const char* kWindowName = "MediaPipe";
		const char* kOutputLandmarks = "pose_landmarks";

		bool m_init = false;
		mediapipe::CalculatorGraph m_graph;
		std::unique_ptr<mediapipe::OutputStreamPoller> p_poller;
		std::unique_ptr<mediapipe::OutputStreamPoller> p_poller_landmarks;
        POSEInfo* m_pose_infos;
        int m_count;
	};
}

#endif
