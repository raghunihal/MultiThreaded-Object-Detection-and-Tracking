#ifndef OBJECT_TRACKER_H
#define OBJECT_TRACKER_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "tracking_msg.h"
#include <tracker.h>
#include <tbb/concurrent_queue.h>
using namespace tbb;

class ObjectTracker
{
  public:
    // Constructor / Destructor
    ObjectTracker();
    ~ObjectTracker();  
    // Method to track object in the given frame
    void trackObject(std::vector<Point2D> &input, float &delT);
    // Method to track object in the queue having frames
    void trackObjectInQueue(concurrent_queue<TrackingMsg> &msgq, concurrent_queue<cv::Mat> &outq);
  private:    
    // Tracker object
    Tracker tracker;
    // Number of frames tracked
    int _numFramesTracked;
    // Add tracking info to the frame
    void addTrackingInfoToFrame(cv::Mat &frame, std::vector<Point2D> &measured, std::vector<Point2D> &tracked,Point2D Average);
};
#endif
