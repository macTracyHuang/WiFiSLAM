/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2020 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>

#include<opencv2/core/core.hpp>

#include <wifi_scan/Fingerprint.h>
#include"../../../include/System.h"

using namespace std;

class WiFiGrabber
{
public:
    WiFiGrabber(ORB_SLAM3::System* pSLAM):mpSLAM(pSLAM){}

    void GrabWifi(const wifi_scan::FingerprintConstPtr& msgWifi);
    // void fingerprint_Callback(const wifi_scan::FingerprintConstPtr& msgWifi);
    queue<wifi_scan::FingerprintConstPtr> wifiBuf;
    std::mutex mBufMutex;
    ORB_SLAM3::System* mpSLAM;
    // ORB_SLAM3::Fingerprint msgToFp(const wifi_scan::FingerprintConstPtr& msgWifi);
    ORB_SLAM3::Fingerprint::FingerprintPtr msgToFp(const wifi_scan::FingerprintConstPtr& msgWifi);
};

class ImageGrabber
{
public:
    ImageGrabber(ORB_SLAM3::System* pSLAM, WiFiGrabber *pWifiGb):mpSLAM(pSLAM) , mpWifiGb(pWifiGb){}

    void GrabImage(const sensor_msgs::ImageConstPtr& msg);

    ORB_SLAM3::System* mpSLAM;
    WiFiGrabber* mpWifiGb;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Mono");
    ros::start();

    if(argc != 3)
    {
        cerr << endl << "Usage: rosrun ORB_SLAM3 Mono path_to_vocabulary path_to_settings" << endl;        
        ros::shutdown();
        return 1;
    }    

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    // ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::MONOCULAR,true);
    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::MONOCULAR,false);


    WiFiGrabber wifigb(&SLAM);
    ImageGrabber igb(&SLAM, &wifigb);

    ros::NodeHandle nodeHandler;
    // ros::Subscriber sub = nodeHandler.subscribe("/camera/image_raw", 1, &ImageGrabber::GrabImage,&igb);
    ros::Subscriber sub = nodeHandler.subscribe("/camera/color/image_raw", 1, &ImageGrabber::GrabImage,&igb);
    ros::Subscriber wifi_sub = nodeHandler.subscribe("/wifi_fp",100, &WiFiGrabber::GrabWifi, &wifigb);

    ros::spin();

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    SLAM.SaveKeyFrameWiFiTrajectoryTUM("KeyFrameWiFiTrajectory.txt");
    SLAM.SaveApInfo("APINFO.txt");
    SLAM.SaveApPlot("APPLOT.csv");

    ros::shutdown();
    cout <<  "end the program"<<endl;
    return 0;
}

void ImageGrabber::GrabImage(const sensor_msgs::ImageConstPtr& msg)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvShare(msg);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }


    wifi_scan::FingerprintConstPtr msgWifi;

    mpWifiGb->mBufMutex.lock();
    //  && (floor(mpWifiGb->wifiBuf.back()->header.stamp.toSec()) == floor(cv_ptrRGB->header.stamp.toSec()))
    if (mpWifiGb->wifiBuf.empty())
    {
        ;
    }
    else if (round(mpWifiGb->wifiBuf.back()->header.stamp.toSec() * 10.0) / 10.0 == round(cv_ptr->header.stamp.toSec() * 10.0) / 10.0)
    {
        msgWifi = mpWifiGb->wifiBuf.back();
    }
    mpWifiGb->mBufMutex.unlock();

    if (msgWifi)
    {
        // cout << "==============WIFI ====================="<<endl;
        cout << msgWifi <<endl;
        mpSLAM->TrackMonocular_Wifi(cv_ptr->image,cv_ptr->header.stamp.toSec(),mpWifiGb->msgToFp(msgWifi));
        // cout << "==============End WIFI ====================="<<endl;
    }
    else
        mpSLAM->TrackMonocular(cv_ptr->image,cv_ptr->header.stamp.toSec());
   
}

void WiFiGrabber::GrabWifi(const wifi_scan::FingerprintConstPtr& msgWifi)
{
    mBufMutex.lock();

    if (!wifiBuf.empty())
        wifiBuf.pop();

    if (!msgWifi->list.empty())
    {
        // cout << "push wifi: "<<setprecision(20) << msgWifi->header.stamp.toSec() <<endl;
        wifiBuf.push(msgWifi);
    }
    // else
    //     cout << setprecision(20) << msgWifi->header.stamp.toSec() << " not push empty wifi" <<endl;
    
    mBufMutex.unlock();
}

ORB_SLAM3::Fingerprint::FingerprintPtr WiFiGrabber::msgToFp(const wifi_scan::FingerprintConstPtr& msgWifi)
{

    // ORB_SLAM3::Fingerprint* fingerprint = new ORB_SLAM3::Fingerprint();
    ORB_SLAM3::Fingerprint::FingerprintPtr fingerprint(new ORB_SLAM3::Fingerprint());

    int rssi_th = -60;
    for (auto &addrssi: msgWifi->list)
    {
        string bssid = addrssi.address;
        int rssi = addrssi.rssi;
        // only remain rssi threshold
        if (rssi < rssi_th)
            continue;
            
        boost::shared_ptr< ::ORB_SLAM3::Ap> ap =  mpSLAM->GetApByBssid(bssid);
        if (!ap)
        {
            boost::shared_ptr< ::ORB_SLAM3::Ap> mpNewAp(new ORB_SLAM3::Ap(bssid));
            mpSLAM->AddNewAp(mpNewAp);
            mpNewAp->nObs = 0;
            mpNewAp->mnId = mpSLAM->GetAllAps().size() + 1;
            Eigen::Vector3f pos(0,0,0);
            mpNewAp->SetApPos(pos);

            fingerprint->mvAp.push_back(mpNewAp);
            fingerprint->mvRssi.push_back(rssi);
        }
        else
        {
            fingerprint->mvAp.push_back(ap);
            fingerprint->mvRssi.push_back(rssi);
        }

    }

    assert(fingerprint->mvAp.size() == fingerprint->mvRssi.size());

    return fingerprint;
}
