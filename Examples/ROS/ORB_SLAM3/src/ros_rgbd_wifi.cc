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
#include <cmath>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include<opencv2/core/core.hpp>

#include"../../../include/System.h"
#include"../../Thirdparty/wifi_scan/msg_gen/cpp/include/wifi_scan/Fingerprint.h"

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
    ORB_SLAM3::Fingerprint msgToFp(const wifi_scan::FingerprintConstPtr& msgWifi);
};

class ImageGrabber
{
public:
    ImageGrabber(ORB_SLAM3::System* pSLAM, WiFiGrabber *pWifiGb):mpSLAM(pSLAM) , mpWifiGb(pWifiGb){}

    void GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);
    void GrabRGBD_Wifi(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);

    ORB_SLAM3::System* mpSLAM;
    WiFiGrabber* mpWifiGb; 
};




int main(int argc, char **argv)
{
    ros::init(argc, argv, "RGBD");
    ros::start();

    if(argc != 3)
    {
        cerr << endl << "Usage: rosrun ORB_SLAM3 RGBD path_to_vocabulary path_to_settings" << endl;        
        ros::shutdown();
        return 1;
    }    

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::RGBD,true);

    WiFiGrabber wifigb(&SLAM);
    ImageGrabber igb(&SLAM, &wifigb);

    ros::NodeHandle nh;

    // message_filters::Subscriber<sensor_msgs::Image> rgb_sub(nh, "/camera/rgb/image_raw", 100);
    // message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/camera/depth_registered/image_raw", 100);
    // 以下为tum rgbd 数据集的topic
    message_filters::Subscriber<sensor_msgs::Image> rgb_sub(nh, "/camera/color/image_raw", 100);
    message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/camera/aligned_depth_to_color/image_raw", 100);
    ros::Subscriber wifi_sub = nh.subscribe("/wifi_fp",100, &WiFiGrabber::GrabWifi, &wifigb);
    // message_filters::Subscriber<wifi_scan::Fingerprint> wifi_sub(nh, "/wifi_fp", 100);
    // typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image, wifi_scan::Fingerprint> sync_pol;
    // message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub, wifi_sub);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub);
    // sync.registerCallback(boost::bind(&ImageGrabber::GrabRGBD_Wifi,&igb,_1,_2,_3));
    sync.registerCallback(boost::bind(&ImageGrabber::GrabRGBD_Wifi,&igb,_1,_2));
    
    ros::spin();

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    // SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    
    // SLAM.SavePointcloudMap();

    ros::shutdown();

    return 0;
}

void ImageGrabber::GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    try
    {
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrD;
    try
    {
        cv_ptrD = cv_bridge::toCvShare(msgD);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    // tm demo block frame
    
    // const double block_time = 1661769533;
    // if (mpSLAM->GetWifiMode() && floor(cv_ptrRGB->header.stamp.toSec()) >= block_time && floor(cv_ptrRGB->header.stamp.toSec()) < block_time + 4)
    // {
    //     cv::Mat blackImage = cv::Mat::zeros(cv::Size(640,480),CV_8UC1);
    //     mpSLAM->TrackRGBD(blackImage,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
    // }
    // // tm
    // else
    // cout << msgWifi->header.stamp.toSec() << ': ' << msgWifi->list[0].rssi <<endl;
    mpSLAM->TrackRGBD(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
}

void ImageGrabber::GrabRGBD_Wifi(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{
        // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    try
    {
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrD;
    try
    {
        cv_ptrD = cv_bridge::toCvShare(msgD);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    // tm demo block frame
    
    // const double block_time = 1661769533;
    // if (mpSLAM->GetWifiMode() && floor(cv_ptrRGB->header.stamp.toSec()) >= block_time && floor(cv_ptrRGB->header.stamp.toSec()) < block_time + 4)
    // {
    //     cv::Mat blackImage = cv::Mat::zeros(cv::Size(640,480),CV_8UC1);
    //     mpSLAM->TrackRGBD(blackImage,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
    // }
    // // tm
    // else

    // To do: decide when wifibuf should pop
    wifi_scan::FingerprintConstPtr msgWifi;

    mpWifiGb->mBufMutex.lock();
    //  && (floor(mpWifiGb->wifiBuf.back()->header.stamp.toSec()) == floor(cv_ptrRGB->header.stamp.toSec()))
    if (mpWifiGb->wifiBuf.empty())
    {
        ;
    }
    else if (round(mpWifiGb->wifiBuf.back()->header.stamp.toSec() * 10.0) / 10.0 == round(cv_ptrRGB->header.stamp.toSec() * 10.0) / 10.0)
    {
        // cout << setprecision(20) << mpWifiGb->wifiBuf.back()->header.stamp.toSec() << endl;
        // cout << mpWifiGb->wifiBuf.back()->list.size() <<endl;
        msgWifi = mpWifiGb->wifiBuf.back();
    }
    mpWifiGb->mBufMutex.unlock();

    // mpSLAM->TrackRGBD(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
    // transfer ros msg to fingerprint.h ap.h

    if (msgWifi)
        mpSLAM->TrackRGBD_Wifi(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec(),mpWifiGb->msgToFp(msgWifi));
    else
        mpSLAM->TrackRGBD_Wifi(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
    
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

ORB_SLAM3::Fingerprint WiFiGrabber::msgToFp(const wifi_scan::FingerprintConstPtr& msgWifi)
{

    ORB_SLAM3::Fingerprint fingerprint = ORB_SLAM3::Fingerprint();

    for (auto &addrssi: msgWifi->list)
    {
        string bssid = addrssi.address;
        int rssi = addrssi.rssi;
        ORB_SLAM3::Ap* ap =  mpSLAM->GetApByBssid(bssid);
        if (!ap)
        {
            ORB_SLAM3::Ap* mpNewAp = new ORB_SLAM3::Ap(bssid);
            mpSLAM->AddNewAp(mpNewAp);
            ap = mpNewAp;
        }
        fingerprint.mvAp.push_back(ap);
        fingerprint.mvRssi.push_back(rssi);
    }

    return fingerprint;
}


