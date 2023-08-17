#include <iostream>
#include <future>
#include <string>
#include "frame_grabber.h"
#include "yolov3v4_object_detector.h"
//#include "yolov5_object_detector.h"
#include "frame_writer.h"
#include "object_tracker.h"
#include <tbb/concurrent_queue.h>
#include "tracking_msg.h"
#include "PTZController.h"

using namespace tbb;

int main() 
{    
    // bool track = true;
    // bool detect = true;
    bool TVflag = true;
    bool IRflag = false;
    char buff[30];  
  

    // load input video path
    //std::string video_input = "../videos/input_video.mp4";
    //std::string video_input1 = "rtsp://service:pssw0rd@172.196.129.152:554/ufirststream?inst=2";
    std::string video_input1 = "rtsp://172.196.128.151:554/1/h264minor";    
    //std::string video_input1 = "rtsp://admin:admin@172.196.128.227:554/videoinput_1:0/mjpeg_3/media.stm";
    //std::string video_input1 = "rtsp://admin:admin@172.196.128.227:554/videoinput_1:0/h264_1/media.stm";    
    //std::string video_input2 = "rtsp://admin:admin@172.196.128.227:554/videoinput_2:0/mjpeg_3/media.stm";
    

    // load output video path
    std::string video_output1 = "../videos/project_detect.avi";
    std::string video_output2 = "../videos/project_detect2.avi";
    //std::string video_output2 = "../videos/project_track_and_detect.avi";
    /*************************YOLO V3***************************/
    // load model config path
    std::string model_configv3 = "../model/yolov3.cfg";
    // load model weights path
    std::string model_weightsv3 = "../model/yolov3.weights";
    /*************************YOLO V4***************************/
    // load model config path
    std::string model_configv4 = "../model/yolov4.cfg";
    // load model weights path
    std::string model_weightsv4 = "../model/yolov4.weights";
    // load class names path
    std::string class_names = "../model/coco.names";
    /*************************YOLO V5***************************/
    // load model config path
    std::string model_configV5 = "../model/yolov5s.onnx";
    // load class names path
    std::string class_namesV5 = "../model/classes.txt";


    // Initialize Yolo v3 & v4 object detector object
    YOLODetector yolo(model_weightsv3, model_configv3, class_names, 0.5, 0.4);
 

    //Initialize PTZ Controller object
    sprintf(buff,"%s:8000","172.196.128.151");    
    // PTZController ptz(buff);
    // PTZController ptz2(buff);
    // ptz.getToken();
    

   
    if(TVflag)
    {
      // Initialize frame grabber object
      FrameGrabber fg(video_input1);
      //Initialize object tracker object
      ObjectTracker trck;
      //Initialize the input and output message queue
      concurrent_queue<cv::Mat> in_queue1;
      concurrent_queue<cv::Mat> out_queue1;
      // Initialize the tracking message queue
      concurrent_queue<TrackingMsg> track_queue1;
      //Initialize Yolov5 object detector object
      //YOLOv5Detector yolo5(model_configV5, class_namesV5, 0.5, 0.4);      
      
      // Initialize the frame writer object
      CustomFrameWriter fw(video_output1, fg.getNumberOfFrames(), fg.getFrameRate(), fg.getFrameSize());
      // Start the task to load the frames
      std::thread ftr_grab_frame(&FrameGrabber::grabFramesFromVideo, &fg , std::ref(in_queue1));
      // Start the task to detect the frames    
      //std::thread ftr_detect_frame(&YOLOv5Detector::detectObjectInQueueAndTrack , &yolo5 , std::ref(in_queue1), std::ref(track_queue1));      
      std::thread ftr_detect_frame(&YOLODetector::detectObjectInQueueAndTrack , &yolo , std::ref(in_queue1), std::ref(track_queue1));      
      // Start the task to track the frames
      std::thread ftr_track_frame(&ObjectTracker::trackObjectInQueue , &trck , std::ref(track_queue1), std::ref(out_queue1));
      // Start the task to write the frames
      std::thread ftr_write_frame(&CustomFrameWriter::writeFramesFromQueueToVideo , &fw , std::ref(out_queue1));

      // synchronize threads:
      ftr_write_frame.join(); 
      ftr_track_frame.join();
      ftr_detect_frame.join();              
      ftr_grab_frame.join();
    }
    else
   if(IRflag)
   {
    // cout<<"IR CODE"<<endl;

    // // Initialize frame grabber object
    // FrameGrabber fg2(video_input2);
    // //Initialize object tracker object
    // ObjectTracker trck2;
    // //Initialize the input and output message queue
    // concurrent_queue<cv::Mat> in_queue2;
    // concurrent_queue<cv::Mat> out_queue2;
    // // Initialize the tracking message queue
    // concurrent_queue<TrackingMsg> track_queue2;
    // //Initialize Yolov5 object detector object
    // YOLOv5Detector yolo5_2(model_configV5, class_namesV5, 0.5, 0.4);      
    
    // // Initialize the frame writer object
    // CustomFrameWriter fw2(video_output1, fg2.getNumberOfFrames(), fg2.getFrameRate(), fg2.getFrameSize());
    // // Start the task to load the frames
    // std::thread ftr_grab_frame2(&FrameGrabber::grabFramesFromVideo, &fg2 , std::ref(in_queue2));
    // // Start the task to detect the frames
    // //std::thread ftr_detect_frame2(&YOLOv5Detector::detectObjectInQueue , &yolo5_2 , std::ref(in_queue2), std::ref(out_queue2));
    // std::thread ftr_detect_frame2(&YOLOv5Detector::detectObjectInQueueAndTrack , &yolo5_2 , std::ref(in_queue2), std::ref(track_queue2)); 
    // // Start the task to track the frames
    // std::thread ftr_track_frame2(&ObjectTracker::trackObjectInQueue , &trck2 , std::ref(track_queue2), std::ref(out_queue2), std::ref(ptz));
    // // Start the task to write the frames
    // std::thread ftr_write_frame2(&CustomFrameWriter::writeFramesFromQueueToVideo2 , &fw2 , std::ref(out_queue2));

    // // synchronize threads:
    // ftr_write_frame2.join();  
    // ftr_track_frame2.join();   
    // ftr_detect_frame2.join();              
    // ftr_grab_frame2.join();

   }
  cout<<"End of program";
  return 0;
}
