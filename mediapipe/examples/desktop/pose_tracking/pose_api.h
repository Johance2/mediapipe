// pose_api.h  定义动态库接口
#if defined(_WIN32)
#define POSE_API __declspec(dllexport)
#endif

struct  POSEInfo;
typedef void(*PoseFrameCallBack)(POSEInfo* infos, int count);

extern "C" POSE_API int POSE_INIT(char* model);

extern "C" POSE_API int POSE_DETECT_VIDEO(const char* video, int show_image, PoseFrameCallBack call);

extern "C" POSE_API int POSE_DETECT_VIDEO2(int rows, int cols, unsigned char *data, POSEInfo** infos, int &count);

extern "C" POSE_API int POSE_RELEASE();

