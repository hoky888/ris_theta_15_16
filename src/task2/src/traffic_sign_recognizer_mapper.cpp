#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <nav_msgs/Odometry.h>
#include <angles/angles.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <sound_play/sound_play.h>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <list>
#include <limits>
#include <utility>
#include <iterator>
#include <detection_msgs/Detection.h>
#include <face_recognition/FRClientGoal.h>
#include <face_recognition/FaceRecognitionAction.h>
#include <math.h>      

struct traffic_sign
{
	std::string name;
	double x,y;
};

std::vector<traffic_sign> signs;
ros::Publisher _pub;

void callback(const detection_msgs::Detection& msg)
{
  ROS_INFO("Traffic sign detected with confidence %f", msg.confidence);
}


void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	for(int i = 0; i < signs.size(); i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "map";
		marker.header.stamp = ros::Time();
		marker.ns = "recognized_traffic_signs_markers";
		marker.id = i;
		marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
		marker.action = visualization_msgs::Marker::ADD;
		marker.text = signs[i].name;
		marker.pose.position.x = signs[i].x;
		marker.pose.position.y = signs[i].y;
		marker.pose.position.z = 0.2;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		marker.scale.x = 0.2;
		marker.scale.y = 0.2;
		marker.scale.z = 0.2;

		marker.color.a = 1.0;
		marker.color.r = 0.0;
		marker.color.g = 1.0;
		marker.color.b = 1.0;
		marker_array.markers.push_back(marker);
	}
	_pub.publish( marker_array );
}

void create(std::string name, double x, double y)
{
	traffic_sign s;
	s.name = name;
	s.x = x;
	s.y = y;
	signs.push_back(s);	
}

int main(int argc, char** argv){
	ros::init(argc, argv, "traffic_sign_recognizer_mapper");
	ros::NodeHandle n;
	
	create("horn", 0.2, -2.0);
	create("mandatory-left", 1.75, 0.7);
	create("speed-limit", 1.2, -0.7);
	create("one-way", 2.8, 0.2);
	create("stop", 4.6, 1.0);
	create("one-way", 3.7, 2.3);

	sleep(1);

	ros::Subscriber sub = n.subscribe("/detector/traffic_signs", -1, callback); 
	_pub = n.advertise<visualization_msgs::MarkerArray>("recognized_traffic_signs", 1000);

  ros::Rate loop_rate(3);
  while (ros::ok())
  {
		publish_waypoints();
    ros::spinOnce();
    loop_rate.sleep();
  }  

  return 0;
}

