cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
project(ObjectDetectionAndTracking)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread -g")

include_directories(include 
                    ${PROJECT_SOURCE_DIR}/include 
                    ${PROJECT_SOURCE_DIR}/include/Detector 
                    ${PROJECT_SOURCE_DIR}/include/Tracker
                    )

find_package(OpenCV 4.5.5 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(TBB REQUIRED)
find_package(CURL REQUIRED) 
include_directories(${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

add_executable(detect_and_track src/main.cpp 
                                src/frame_grabber.cpp 
                                src/yolov3v4_object_detector.cpp
				src/yolov5_object_detector.cpp
                                src/frame_writer.cpp
                                src/hungarian.cpp
                                src/kalman_filter.cpp
                                src/kalman_track.cpp
                                src/tracker.cpp
                                src/object_tracker.cpp
                                )

target_link_libraries(detect_and_track PRIVATE TBB::tbb ${OpenCV_LIBRARIES} ${EIGEN3_DIR} ${CURL_LIBRARIES})
