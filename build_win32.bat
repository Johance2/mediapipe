set BAZEL_VS=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools
set BAZEL_VC=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC
set BAZEL_VC_FULL_VERSION=14.29.30133
set BAZEL_WINSDK_FULL_VERSION=10.0.19041.0

set BAZEL_SH=C:\msys64\usr\bin\bash.exe

::bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="C://Users/Administrator//AppData//Local//Programs//Python//Python311//python.exe" mediapipe/examples/desktop/hello_world
::bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="C://Users/Administrator//AppData//Local//Programs//Python//Python311//python.exe" mediapipe/examples/desktop/pose_tracking:pose_tracking_cpu
bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="C://Users/Administrator//AppData//Local//Programs//Python//Python311//python.exe" mediapipe/examples/desktop/pose_tracking:POSE_DLL



::set GLOG_logtostderr=1

::bazel-bin\mediapipe\examples\desktop\hello_world\hello_world.exe
::bazel-bin\mediapipe\examples\desktop\pose_tracking\pose_tracking_cpu --calculator_graph_config_file=mediapipe/graphs/pose_tracking/pose_tracking_cpu.pbtxt --input_video_path=test.mp4 --output_video_path=test_output_cpu.mp4
