#ifndef YOLO_DETECTOR_H
#define YOLO_DETECTOR_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include "point2D.h"
#include "tracking_msg.h"

#include <tbb/concurrent_queue.h>
using namespace tbb;

typedef struct 
{
    int class_id;
    float confidence;
    cv::Rect box;
}Detection;

// class which provides functionality to detect object using YOLO algorithm
class YOLOv5Detector  
{
  public:

    // Constructor and Destructor
    YOLOv5Detector(std::string modelConfig, std::string classesFilename, float confThresh, float nmsThresh);
    ~YOLOv5Detector();
    // Method to detect object in the msgq having frames and put the detected frames in the outq
    void detectObjectInQueue(concurrent_queue<cv::Mat> &msgq, concurrent_queue<cv::Mat> &outq);
    // Method to detect object in the msgq having frames and put the detected frame and detected points in trackq
    void detectObjectInQueueAndTrack(concurrent_queue<cv::Mat> &msgq, concurrent_queue<TrackingMsg> &trackq);
    // Method to detect object in the given frame
    void detectObjectv5(cv::Mat &frame, std::vector<Detection> &output, const std::vector<std::string> &className);
    cv::Mat format_Yolov5(const cv::Mat &source);


    // Function to load the class names in a vector
    inline void loadClasses(std::string classesFilename)
    {
      std::fstream file(classesFilename);
      std::string line;
      while(std::getline(file, line))
      {
        _classes.push_back(line);
      }
      std::cout << "Loaded " << _classes.size() <<" Class Names" << std::endl;
    }

    // Confidence threshold for class
    float _confidenceThresh;

    // Non-max Suppression threshold to remove the access bounding boxes
    float _nonMaxSuppressionThresh;

    // Unique Pointer holding the Object Dectector
    std::unique_ptr<cv::dnn::Net> _detector;
    // Vector containing class names
    std::vector<std::string> _classes;
  protected:    
    // Method to add inference time to detected object frame
    void addDetectionTimeToFrame(cv::Mat &frame);
  private:    
    // Input width and height of the image to the network
    int _inputWidth;
    int _inputHeight;
    float _frameDetectionTime;
    std::vector<Point2D> _objectCenters;
};

#endif
