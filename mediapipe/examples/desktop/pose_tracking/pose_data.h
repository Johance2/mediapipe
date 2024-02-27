#pragma once
struct  POSEInfo
{
  float x;
  float y;
  float z;
  bool v;
  bool p;
};

#define POSE_RESULT_OK 0
#define POSE_RESULT_GRAPH_INIT_EXCEPTION 1
#define POSE_RESULT_RUN_GRAPH_EXCEPTION 2
#define POSE_RESULT_RELEASE_EXCEPTION 3
#define POSE_RESULT_NOT_INIT 4

// # 0 - nose
// # 1 - left eye (inner)
// # 2 - left eye
// # 3 - left eye (outer)
// # 4 - right eye (inner)
// # 5 - right eye
// # 6 - right eye (outer)
// # 7 - left ear
// # 8 - right ear
// # 9 - mouth (left)
// # 10 - mouth (right)
// # 11 - left shoulder
// # 12 - right shoulder
// # 13 - left elbow
// # 14 - right elbow
// # 15 - left wrist
// # 16 - right wrist
// # 17 - left pinky
// # 18 - right pinky
// # 19 - left index
// # 20 - right index
// # 21 - left thumb
// # 22 - right thumb
// # 23 - left hip
// # 24 - right hip
// # 25 - left knee
// # 26 - right knee
// # 27 - left ankle
// # 28 - right ankle
// # 29 - left heel
// # 30 - right heel
// # 31 - left foot index
// # 32 - right foot index
enum PoseType
{
    Nose = 0,
    LeftEyeInner = 1,
    LeftEye = 2,
    LeftEyeOuter = 3,
    RightEyeInner = 4,
    RightEye = 5,
    RightEyeOuter = 6,
    LeftEar = 7,
    RightEar = 8,
    MouthLeft = 9,
    MouthRight = 10,
    LeftShoulder = 11,
    RightShoulder = 12,
    LeftElbow = 13,
    RightElbow = 14,
    LeftWrist = 15,
    RightWrist = 16,
    LeftPinky = 17,
    RightPinky = 18,
    LeftIndex = 19,
    RightIndex = 20,
    LeftThumb = 21,
    RightThumb = 22,
    LeftHip = 23,
    RightHip = 24,
    LeftKnee = 25,
    RightKnee = 26,
    LeftAnkle = 27,
    RightAnkle = 28,
    LeftHeel = 29,
    RightHeel = 30,
    LeftFootIndex = 31,
    RightFootIndex = 32
};


typedef int POSEResult;