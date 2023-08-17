/* 
 * File:   PTZController.h
 * Author: Raghunath N
 * Created on March 25, 2022, 16:50 PM
 */

#ifndef PTZCONTROLLER_H
#define PTZCONTROLLER_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;

class PTZController
{
private:
    string cameraToken = "PROFILE_CH1_STREAM1";
    string camUrl = "";

    static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string *s)
    {
        size_t newLength = size*nmemb;
        size_t oldLength = s->size();
        try
        {
            s->resize(oldLength + newLength);
        }
        catch(std::bad_alloc &e)
        {
            //handle memory problem
            return 0;
        }

        std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
        return size*nmemb;
    }

public:    
    PTZController(string ip)
    {
        camUrl = "http://" + ip + "/onvif/device_service";
        //cout<<camUrl<<endl;
    }
    
    string post(string content)
    {
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        return respondContent;
    }

    string getToken()
    {
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            string strGetProfile = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:tt=\"http://www.onvif.org/ver10/schema\"><s:Header><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">fx9HwLo3/A095YMB6m2/O367Jzs=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">ezE2NTY4OWM0LWI4ZDctNDllOC1hMDA2LWQ2ZmY2NjEwN2Q3NX0=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-04T09:07:00Z</Created></UsernameToken></Security></s:Header><s:Body><tds:GetCapabilities><tds:Category>All</tds:Category></tds:GetCapabilities></s:Body></s:Envelope>";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strGetProfile.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            //std::cout<<"Returned:"<<respondContent;
        }
        return "";
    } 
    void stop()
    {
        cout<<"Stop Command Recieved"<<endl;
        post("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:d=\"http://www.w3.org/2001/XMLSchema\" xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"><s:Header><Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver20/ptz/wsdl/Stop</Action><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">lH51QSDTv9rgkDmoT5WgslQVcmY=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">e2VhNzc5MzJlLTE3NzItNDdmNS04ZDE0LTZjOGMzNWRmMjIxMH0=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-04T14:45:00Z</Created></UsernameToken></Security></s:Header><s:Body><Stop xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\"><ProfileToken>PROFILE_CH1_STREAM1</ProfileToken><PanTilt>true</PanTilt><Zoom>true</Zoom></Stop></s:Body></s:Envelope>");
    }
    
    void GotoHome()
    {
        cout<<"GotoHome Command Recieved"<<endl;
        post("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:d=\"http://www.w3.org/2001/XMLSchema\" xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"><s:Header><Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver20/ptz/wsdl/GotoHomePosition</Action><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">EFFjnRs5AmP4Fo4TkCCBsPwssnU=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">ezU1Njk1MGQzLWQ4Y2ItNGMwOC04Y2NiLTEwOWY2ZWM1OGMwM30=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-04T15:47:26Z</Created></UsernameToken></Security></s:Header><s:Body><GotoHomePosition xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\"><ProfileToken>PROFILE_CH1_STREAM1</ProfileToken></GotoHomePosition></s:Body></s:Envelope>");
    }


    void move(float x, float y)
    {        
        //cout<<std::to_string(x)<<std::endl;
        //cout<<std::to_string(y)<<std::endl;        
        string dirCmd = "<PanTilt xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + std::to_string(x) + "\" y=\""+ std::to_string(y) +"\"/>";
        string cmd = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:d=\"http://www.w3.org/2001/XMLSchema\" xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"><s:Header><Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver20/ptz/wsdl/ContinuousMove</Action><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">xRYkaQ96arR0MPWMykSt8IVVxWY=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">ezcwYzljZjUwLWI2OWItNDk3NC1hNWI3LWY4OThlNGFiYmMwN30=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-04T11:35:19Z</Created></UsernameToken></Security></s:Header><s:Body><ContinuousMove xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\"><ProfileToken>PROFILE_CH1_STREAM1</ProfileToken><Velocity> " + dirCmd + "<Zoom xmlns=\"http://www.onvif.org/ver10/schema\" x=\"0\"/></Velocity></ContinuousMove></s:Body></s:Envelope>";
        post(cmd);        
    } 

    void absoluteMove(float x, float y,float valueZoom)
    {        
        //cout<<std::to_string(x)<<std::endl;
        //cout<<std::to_string(y)<<std::endl;        
        string dirCmd = "<PanTilt xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + std::to_string(x) + "\" y=\""+ std::to_string(y) +"\"/>";
        string cmd = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:d=\"http://www.w3.org/2001/XMLSchema\" xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"><s:Header><Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver20/ptz/wsdl/AbsoluteMove</Action><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">YUwMm7dwoNBh028j1H0SQC2M/Q4=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">ezlmYTc3MmU2LTllMWQtNDgyYi1hZGY5LTA3NDg1YjcxMzNhMX0=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-21T14:45:09Z</Created></UsernameToken></Security></s:Header><s:Body><AbsoluteMove xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\"><ProfileToken>PROFILE_CH1_STREAM1</ProfileToken><Position> " + dirCmd + "<Zoom xmlns=\"http://www.onvif.org/ver10/schema\" x=\"0\"/></Position><Speed><PanTilt x=\"1\" y=\"1\"/><Zoom x=\"1\"/></Speed></AbsoluteMove></s:Body></s:Envelope>";
        post(cmd);    
    }

    void relativeMove(float x, float y,float valueZoom)
    {
        //cout<<std::to_string(x)<<std::endl;
        //cout<<std::to_string(y)<<std::endl;        
        string dirCmd = "<PanTilt xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + std::to_string(x) + "\" y=\""+ std::to_string(y) +"\"/>";
        string cmd = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:d=\"http://www.w3.org/2001/XMLSchema\" xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"><s:Header><Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver20/ptz/wsdl/RelativeMove</Action><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">jSEs+YYCRyNU9cz6BXjXTLwpijM=</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">e2IzYTUyMmZhLTk5YTQtNDc3My04NzE5LTk2ZTdhYjQ2Nzc3YX0=</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-04-21T14:52:30Z</Created></UsernameToken></Security></s:Header><s:Body><RelativeMove xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\"><ProfileToken>PROFILE_CH1_STREAM1</ProfileToken><Translation> " + dirCmd + "<Zoom xmlns=\"http://www.onvif.org/ver10/schema\" x=\"0.1\"/></Translation></RelativeMove></s:Body></s:Envelope>";
        post(cmd);           
    }

   
};
#endif /* PTZCONTROLLER_H */


