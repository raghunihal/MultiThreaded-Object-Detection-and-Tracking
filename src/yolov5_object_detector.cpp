#include "yolov5_object_detector.h"


/****************************************************************************************************************/
const std::vector<cv::Scalar> colors = {cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 0)};
// Constructor
YOLOv5Detector::YOLOv5Detector(std::string modelConfig, std::string classesFilename, float confThresh, float nmsThresh)
{    
    _confidenceThresh = confThresh;
    _nonMaxSuppressionThresh = nmsThresh;    
    _detector = std::make_unique<cv::dnn::Net>(cv::dnn::readNet(modelConfig));
    _detector->setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    _detector->setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);

    if(_detector->empty())
    {
        std::cout << "ONNX Model Load Error" << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "ONNX Model Loaded Successfully!" << std::endl;
    }
    
    loadClasses(classesFilename);
    
    _inputWidth = 640;
    _inputHeight = 640;
}
/****************************************************************************************************************/
// Destructor
YOLOv5Detector::~YOLOv5Detector(){};
/****************************************************************************************************************/
void YOLOv5Detector::detectObjectInQueue(concurrent_queue<cv::Mat> &inq,concurrent_queue<cv::Mat> &outq)
{
    cv::Mat frame;    
    while(1)
    {            
        if(inq.try_pop(frame))
        {
        std::vector<Detection> output;
        this->detectObjectv5(frame, output, _classes);      
        // Move the detected frame to outer queue
        outq.push(frame);
        }
        //frame.release();
    }
}
/****************************************************************************************************************/
void YOLOv5Detector::detectObjectInQueueAndTrack(concurrent_queue<cv::Mat> &inq, concurrent_queue<TrackingMsg> &trackq)
{
    //long double execTime = 0.0 , prevCount = 0.0, time = 0.0;   
    cv::Mat frame;  
    while(1)
    {               
        if(inq.try_pop(frame))
        {
            std::vector<Detection> output;
            // prevCount = cv::getTickCount() * 1.0000;
            this->detectObjectv5(frame, output, _classes);
            //time += execTime;           
            // std::cout << "execTime = " << execTime << "; time = " << time << std::endl;
            // execTime = (cv::getTickCount()*1.0000 - prevCount) / (cv::getTickFrequency() * 1.0000);
            // Move the detected frame to Tracker queue
            TrackingMsg trackmsg(frame, _objectCenters, _frameDetectionTime);
            trackq.push(std::move(trackmsg));
            _objectCenters.clear();
        }
        //frame.release();
    }
}
/****************************************************************************************************************/
cv::Mat YOLOv5Detector::format_Yolov5(const cv::Mat &source)
{
    int col = source.cols;
    int row = source.rows;
    int _max = MAX(col, row);
    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(cv::Rect(0, 0, col, row)));
    return result;
}
/****************************************************************************************************************/
void YOLOv5Detector::detectObjectv5(cv::Mat &frame, std::vector<Detection> &output, const std::vector<std::string> &className)
{
    // Create blob from the Image
    cv::Mat blob;
    auto input_image = format_Yolov5(frame);    
    cv::dnn::blobFromImage(input_image, blob, 1./255., cv::Size(_inputWidth,_inputHeight), cv::Scalar(0,0,0), true, false);
    // Sets the input to the network
    _detector->setInput(blob);
    std::vector<cv::Mat> outputs;
    // Runs the forward pass through the network to get output of the output layers
    _detector->forward(outputs,_detector->getUnconnectedOutLayersNames());     
    //this->addDetectionTimeToFrame(frame);

    float x_factor = input_image.cols / _inputWidth;
    float y_factor = input_image.rows / _inputHeight;    
    float *data = (float *)outputs[0].data;
    const int dimensions = 85;
    const int rows = 25200;
   
    
    std::vector<int> class_ids;    
    std::vector<float> confidences;    
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i)
    {
        float confidence = data[4];
        if (confidence >= _confidenceThresh)
        {
            float * classes_scores = data + 5;
            cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > 0.2)
            {
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);                
                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];
                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        data += 85;
    }


  // Apply nonmax suppression
   std::vector<int> nms_result;      
   cv::dnn::NMSBoxes(boxes, confidences, 0.2, _nonMaxSuppressionThresh, nms_result);
   for (size_t i = 0; i < nms_result.size(); ++i)
   {	
      int idx = nms_result[i];      
      Detection result;
      result.class_id = class_ids[idx];
      result.confidence = confidences[idx];
      result.box = boxes[idx];
      output.push_back(result);	
      Point2D p;
      p.x = (result.box.x + result.box.width + result.box.x)/2;
      p.y = (result.box.y + result.box.height + result.box.y)/2;
      _objectCenters.push_back(p);
   }
   
   int detections = output.size();
   for (int i = 0; i < detections; ++i)
   {
        auto detection = output[i];
        auto box = detection.box;
        auto classId = detection.class_id;
        const auto color = colors[classId % colors.size()];
        cv::rectangle(frame, box, color, 1);
        cv::rectangle(frame, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color, cv::FILLED);
        cv::putText(frame, className[classId].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
   }
        
}
/****************************************************************************************************************/
void YOLOv5Detector::addDetectionTimeToFrame(cv::Mat &frame)
{
    std::vector<double> layersTimes;    
    float freq = cv::getTickFrequency() / 1000;
    float t = _detector->getPerfProfile(layersTimes) / freq;
    std::string label = "Inference time for the frame : " + std::to_string(t) + " ms";
    cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0));
    _frameDetectionTime = t;
}
/****************************************************************************************************************/


