//pose_api.cpp
#include "pose_api.h"
#include "pose_detect.h"

using namespace POSE_SAMPLE;
pose_detect m_pose;

extern "C" POSE_API int POSE_INIT(char* model) {
	return m_pose.init_graph(model);
}

extern "C" POSE_API int POSE_DETECT_VIDEO(const char* video, int show_image, PoseFrameCallBack call) {
	return m_pose.detect_video(video, show_image, call);
}
extern "C" POSE_API int POSE_DETECT_VIDEO2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count) {
	return m_pose.detect_video2(rows, cols, data, infos, count);
}

extern "C" POSE_API int POSE_RELEASE() {
	return m_pose.release();
}
