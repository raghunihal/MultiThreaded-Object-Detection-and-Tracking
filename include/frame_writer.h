#ifndef FRAME_WRITER_H
#define FRAME_WRITER_H

#include <memory>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <tbb/concurrent_queue.h>
using namespace tbb;

// Class which provides functionality to write the frames as videos or images
class CustomFrameWriter
{
  public:
    // Constructor and Destructor
    CustomFrameWriter(std::string filename, int numFrames, double _frameRate, cv::Size _frameSize);
    ~CustomFrameWriter();

    // Function to capture frames from video
    bool writeFramesFromQueueToVideo(concurrent_queue<cv::Mat> &msgq);
    bool writeFramesFromQueueToVideo2(concurrent_queue<cv::Mat> &msgq);

    
    bool writeFrameToImage(cv::Mat &frame);

  private:
    // Unique pointer holding the video writer object
    std::unique_ptr<cv::VideoWriter> _vWrite;

    // Number of frames in video
    int _numFrames;
};

#endif

