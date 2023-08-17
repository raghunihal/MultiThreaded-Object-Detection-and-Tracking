#include <object_tracker.h>
#include <numeric>
#include "PTZController.h"

// Constructor
ObjectTracker::ObjectTracker()
{
    std::cout << "Initialised the tracker" << std::endl;
    _numFramesTracked = 0;
    // Initialize the object tracker
    tracker.init();
}

// Destructor
ObjectTracker::~ObjectTracker(){};

// TrackObject based on the measured location of the objects
void ObjectTracker::trackObject(std::vector<Point2D> &input, float &delT)
{
    //Update the tracker
    tracker.updateTracks(input, delT);
}

// Track Objects in the queue of Detected frames
void ObjectTracker::trackObjectInQueue(concurrent_queue<TrackingMsg> &trackq, concurrent_queue<cv::Mat> &outq)
{      
     TrackingMsg tracking_msg;
    while(1)
    {                         
        if(trackq.try_pop(tracking_msg))
        {
            // Track object
            trackObject(tracking_msg.p, tracking_msg.delT);
            // Get the predicted tracks of the tracked objects
            std::vector<Point2D> tracked_points = tracker.getTracks();        
            if(tracking_msg.p.size()>0)
            {
                Point2D Average;         	
                for(auto &p: tracking_msg.p)
                {	
                    if((p.x <=1280 && p.x > 0.0) && (p.y <=720 && p.y > 0.0))
                    { 
                        Average.x += p.x;
                        Average.y += p.y;
                    } 

                }
                Average.x = Average.x/(float)tracking_msg.p.size();	
                Average.y = Average.y/(float)tracking_msg.p.size();                
                addTrackingInfoToFrame(tracking_msg.frame, tracking_msg.p, tracked_points, Average);
            }	
        // Send the tracked frames to the queue
        outq.push(std::move(tracking_msg.frame));        
        }
        //tracking_msg.frame.release();
    }
}

void ObjectTracker::addTrackingInfoToFrame(cv::Mat &frame, std::vector<Point2D> &measured, std::vector<Point2D> &tracked,Point2D Average)
{
    // Add red circle to the Tracked location
    for(auto &p: tracked)
    {
        cv::circle(frame, cv::Point(p.x, p.y), 6, cv::Scalar(0,0,255), -1);
    }
    // Add green circle to the detected location
    for(auto &p: measured)
    {
        cv::circle(frame, cv::Point(p.x, p.y), 3, cv::Scalar(0,255,0), -1);
    }
    cv::circle(frame, cv::Point(Average.x, Average.y), 3, cv::Scalar(210,250,0), -1);
}



/*
int deltaX,deltaY;
float scale = 0.2;
int centerX = 1280/2, centerY = 720/2;
char buff[20];
sprintf(buff,"%s:8000",argv[1]);
PTZController ptz(buff);
ptz.getToken();

centerBX =  Average.x;
centerBY = Average.y;
deltaX = centerBX - centerX;
deltaY = centerY - centerBY;

if(abs(sqrt(deltaX*deltaX + deltaY*deltaY)) < 50)  
  scale = 0.2;

if ((abs(deltaY) < 20) && (abs(deltaX) < 20))
{
    ptz.stop();
}
else 
if (abs(deltaY) > abs(deltaX))
{
    cout << ((float)deltaX*scale/abs(deltaY)) << ":::::::" << ((deltaY > 0)?scale:(-scale)) << endl;
    ptz.relativeMove((float)deltaX*scale/abs(deltaY),(deltaY > 0)?scale:(-scale));
}
else
{
    cout << ((deltaX > 0)?scale:(-scale)) <<":::::::"<< ((float)deltaY*scale/abs(deltaX)) << endl;
    ptz.relativeMove((deltaX > 0)?scale:(-scale),(float)deltaY*scale/abs(deltaX));
}
*/