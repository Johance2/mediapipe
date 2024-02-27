# Copyright 2020 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

package(default_visibility = ["//mediapipe/examples:__subpackages__"])

cc_binary(
    name = "pose_tracking_cpu",
    data = [
        "//mediapipe/modules/pose_detection:pose_detection.tflite",
        "//mediapipe/modules/pose_landmark:pose_landmark_full.tflite",
    ],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
        "//mediapipe/graphs/pose_tracking:pose_tracking_cpu_deps",
    ],
)

cc_binary(
	name = "POSE_DLL",
	srcs = ["pose_api.h","pose_api.cpp","pose_detect.h","pose_detect.cpp","pose_data.h"], # 包含项
	linkshared=True, # 生成动态库
	deps = [ 
		"//mediapipe/graphs/pose_tracking:pose_tracking_cpu_deps", #依赖项
	],
)
# Linux only
cc_binary(
    name = "pose_tracking_gpu",
    data = [
        "//mediapipe/modules/pose_detection:pose_detection.tflite",
        "//mediapipe/modules/pose_landmark:pose_landmark_full.tflite",
    ],
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main_gpu",
        "//mediapipe/graphs/pose_tracking:pose_tracking_gpu_deps",
    ],
)
