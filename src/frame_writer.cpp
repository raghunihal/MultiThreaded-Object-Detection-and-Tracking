#include "frame_writer.h"
CustomFrameWriter::CustomFrameWriter(std::string out_filename, int numFrames, double frameRate, cv::Size frameSize):
_numFrames(numFrames)
{
    // Make unique pointer of video writer object using filenama, codec , fps and frame_size
    //_vWrite = std::make_unique<cv::VideoWriter>(out_filename, cv::VideoWriter::fourcc('M','J','P','G'), frameRate, frameSize);

    // Write this string to one line to be sure!!
    //_vWrite.open("appsrc ! videoconvert ! videoscale ! video/x-raw,width=640,height=480
//             ! x264enc speed-preset=veryfast tune=zerolatency bitrate=800 !
//              rtspclientsink location=rtsp://localhost:8554/mystream ",
//               0, 20, Size(640, 480), true);
}

CustomFrameWriter::~CustomFrameWriter()
{
    // Release the video writer object
    _vWrite->release();
}

bool CustomFrameWriter::writeFramesFromQueueToVideo(concurrent_queue<cv::Mat> &outq)
{
    cv::Mat frame;
    while(cv::waitKey(20) != 27)
    {
                
         if(outq.try_pop(frame))
         {            
            //_vWrite->write(frame);            
            cv::imshow("MultiTracker TV",frame);            
         }
         //frame.release();
    }
}

bool CustomFrameWriter::writeFramesFromQueueToVideo2(concurrent_queue<cv::Mat> &outq)
{
    cv::Mat frame;    
    while(cv::waitKey(20) != 27)
    {
                
         if(outq.try_pop(frame))
         {            
            //_vWrite->write(frame);            
            cv::imshow("MultiTracker IR",frame);
         }
         //frame.release();
    }
}


