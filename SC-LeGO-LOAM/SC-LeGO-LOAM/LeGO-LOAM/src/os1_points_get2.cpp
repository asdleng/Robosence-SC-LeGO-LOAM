#include <ros/ros.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Odometry.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/range_image/range_image.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/common.h>
#include <pcl/registration/icp.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
 
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <deque>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cfloat>
#include <iterator>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip>
#include <array>
#include <thread>
#include <mutex>
ros::Publisher velodyne_pub;

void rslidarCallback(const sensor_msgs::PointCloud2::ConstPtr& rs_msg){
    
    pcl::PointCloud<pcl::PointXYZI>::Ptr rs_ptr(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromROSMsg(*rs_msg, *rs_ptr);

    sensor_msgs::PointCloud2 velodyne_points;
    pcl::PointCloud<pcl::PointXYZI>::Ptr velodyne_ptr(new pcl::PointCloud<pcl::PointXYZI>);
    

    std::vector<int> mapping;
    pcl::removeNaNFromPointCloud(*rs_ptr, *velodyne_ptr, mapping);

    pcl::toROSMsg(*velodyne_ptr,velodyne_points);
    velodyne_points.header = rs_msg->header;
    velodyne_pub.publish(velodyne_points);
}
int main(int argc, char **argv){
    ros::init(argc, argv, "rslidar2velodyne");
    ros::NodeHandle nh;
    ros::Subscriber rslidar_sub = nh.subscribe("/rslidar_points",1,rslidarCallback);
    velodyne_pub = nh.advertise<sensor_msgs::PointCloud2>("//os1_points",10);
    ros::spin();
    return 0;
}