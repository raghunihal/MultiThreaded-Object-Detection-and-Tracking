#include "frame_grabber.h"

FrameGrabber::FrameGrabber(std::string RTSP_URL)
{
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;tcp", 1);
    // Make unique pointer using RTSP_URL
    _vcap = std::make_unique<cv::VideoCapture>(RTSP_URL,cv::CAP_FFMPEG);
    // Check if the video file is opened without error
    if(!_vcap->isOpened())
    {
        std::cout << "Error: not able to open Video File/ Video File not Found" << std::endl;
        exit(-1);
    }    
    // Load frame Rate of the video
    _frameRate = _vcap->get(cv::CAP_PROP_FPS);
    std::cout<<"frameRate"<<_vcap->get(cv::CAP_PROP_FPS)<<std::endl;
    // Load video file codec format
    _fourCC = _vcap->get(cv::CAP_PROP_FOURCC);
    std::cout<<"_fourCC"<<_vcap->get(cv::CAP_PROP_FOURCC)<<std::endl; 
    // Load frame size of the video
    _frameSize = cv::Size( _vcap->get(cv::CAP_PROP_FRAME_WIDTH), _vcap->get(cv::CAP_PROP_FRAME_HEIGHT));
    // Load number of frames in video
    //_numFrames = _vcap->get(cv::CAP_PROP_FRAME_COUNT);  

    _vcap->set(cv::CAP_PROP_BUFFERSIZE, 1);

    // _vcap->set(cv::CAP_PROP_EXPOSURE,3);
    // _vcap->set(cv::CAP_PROP_EXPOSURE,1);
    // _vcap->set(cv::CAP_PROP_EXPOSURE,40);

   std::cout<<_vcap->get(cv::CAP_PROP_FRAME_WIDTH)<<std::endl;  
   std::cout<<_vcap->get(cv::CAP_PROP_FRAME_HEIGHT)<<std::endl;  
}

FrameGrabber::~FrameGrabber()
{
    // Release the video capture object
    _vcap->release();
}

void FrameGrabber::grabFramesFromVideo(concurrent_queue<cv::Mat> &msgq)
{
    static int count =0;
     
    // Read and capture the frames untill there is frame in the video stream 
    cv::Mat frame;   
    while(1)
    {
       
        // if(_vcap->grab())
        // {
        //     _vcap->retrieve(frame);           
        //     msgq.push(std::move(frame));            
        // }

        bool OK = _vcap->grab();
        if (OK == false)
        {
            std::cout << "cannot grab" << std::endl;
        }
        else
        {
            // retrieve a frame of your source
            //_vcap->read(frame);
            _vcap->retrieve(frame); 
            if(count%2 ==0)                  
            msgq.push(std::move(frame)); 

            count++;
        }


        //frame.release();
    }
}


