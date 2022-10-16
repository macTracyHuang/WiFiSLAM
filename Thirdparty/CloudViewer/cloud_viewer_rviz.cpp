#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include<ros/ros.h>
#include<pcl/point_cloud.h>
#include<pcl_conversions/pcl_conversions.h>
#include<sensor_msgs/PointCloud2.h>

// typedef pcl::PointXYZ PointT;
typedef pcl::PointXYZRGBA PointT;


int main (int argc, char *argv[])
{
    pcl::PointCloud<PointT> cloud;
    pcl::PointCloud<pcl::PointXYZ> wifi_cloud;

    // Load .pcd file from launch file
    ros::init (argc, argv, "cloud_viewer_rviz");
    std::string pcd_path;
    ros::param::get("~pcd_path", pcd_path);

    int ret = pcl::io::loadPCDFile (pcd_path, cloud);
    if (ret < 0) {
        PCL_ERROR("Couldn't read file %s\n", pcd_path);
        return -1;
    }

    int ret_wifi = pcl::io::loadPCDFile ("/root/ORB_SLAM3/test_wifi.pcd", wifi_cloud);
    if (ret_wifi < 0) {
        PCL_ERROR("Couldn't read file %s\n", wifi_cloud);
        return -1;
    }

    
    ros::NodeHandle nh, wifi_nh;
    ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2> ("pcl_output", 1);
    ros::Publisher wifi_pub = wifi_nh.advertise<sensor_msgs::PointCloud2> ("wifi_output", 1);
    sensor_msgs::PointCloud2 output, wifi_output;

    //Convert the cloud to ROS message
    pcl::toROSMsg(cloud, output);
    pcl::toROSMsg(wifi_cloud, wifi_output);

    output.header.frame_id = "odom";//this has been done in order to be able to visualize our PointCloud2 message on the RViz visualizer
    wifi_output.header.frame_id = "odom";
    ros::Rate loop_rate(1);
    while (ros::ok())
    {
        pcl_pub.publish(output);
        wifi_pub.publish(wifi_output);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}