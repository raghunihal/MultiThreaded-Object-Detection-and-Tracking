# MultiThreaded-Object-Detection-and-Tracking

This repository contains an implementation of a multithreaded application for detecting objects and tracking objects from RTSP Video Stream.

The whole detection and tracking application runs on four tasks. First task start reading frames using Frame Grabber object and pushes them them into a concurrent message queue which is based on Intel TBB. The second tasks detects the frames by pulling the frames from concurrent message queue as they become available using Object Detector. After detection of each frame, the detector puts detected frame along with the required input data for tracking into another queue having a special message is type of tracking messages. The Object Tracker pulls the frames with tracking messages and uses the Tracker which provides functionality to update the tracks. The tracks are implements as abstract class. Each frame after tracking is put in the output queue. After the completion of tracking task, the final task is started using Frame Writer object which pulls the messages from the output queue and Displays them on the window.

# Object Detectors:
This object detector is trained on coco dataset and can recognize upto 80 different classes of objects in the moving frame.
YOLOV3/YOLOV4 Object-Detector or YOLOV5 Object-Detector.

# Trackers: Kalman
The project provides abstract implementation of the tracker in form of tracks, hence any custom object tracker can be added. Currently, kalman tracker is implemented using Kalman-Filter. An assignment problem is used to associate the objects detected by detectors and tracked by trackers. Here it is solved using Hungarian Algorithm.

Tracking consist of two major steps:
1. Prediction: Predict the object locations in the next frame.
2. Data association: Use the predicted locations to associate detections across frames to form tracks.

# Red dot on the objects shows the tracking and the green dot shows the position measured by detecting the object and calculating center of bounding box.
