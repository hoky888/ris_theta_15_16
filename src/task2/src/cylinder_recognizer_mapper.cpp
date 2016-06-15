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

struct cylinder
{
	std::string color;
	double x,y;
} cylinders[4];

ros::Publisher _pub;

void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "map";
		marker.header.stamp = ros::Time();
		marker.ns = "recognized_cylinders_markers";
		marker.id = i;
		marker.type = visualization_msgs::Marker::CYLINDER;
		marker.action = visualization_msgs::Marker::ADD;
		marker.text = cylinders[i].color;
		marker.pose.position.x = cylinders[i].x;
		marker.pose.position.y = cylinders[i].y;
		marker.pose.position.z = 0.1;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		marker.scale.x = 0.2;
		marker.scale.y = 0.2;
		marker.scale.z = 0.2;

		marker.color.a = 1.0; // Don't forget to set the alpha!
		if(cylinders[i].color == "yellow")
		{
			marker.color.r = 1;
			marker.color.g = 1;
			marker.color.b = 0;
		}
		else if(cylinders[i].color == "red")
		{
			marker.color.r = 1;
			marker.color.g = 0;
			marker.color.b = 0;
		}
		else if(cylinders[i].color == "blue")
		{
			marker.color.r = 0;
			marker.color.g = 0;
			marker.color.b = 1;
		}
		else if(cylinders[i].color == "green")
		{
			marker.color.r = 0;
			marker.color.g = 1;
			marker.color.b = 0;
		}

		marker_array.markers.push_back(marker);
	}
	_pub.publish( marker_array );
}


int main(int argc, char** argv){
	ros::init(argc, argv, "cylinder_recognizer_mapper");
	ros::NodeHandle n;

	sleep(1);
		
	_pub = n.advertise<visualization_msgs::MarkerArray>("recognized_cylinders", 1000);
		
	cylinders[0].color = "yellow";
	cylinders[0].x = 2.5;
	cylinders[0].y = 3.0;

	cylinders[1].color = "red";
	cylinders[1].x = 6.0;
	cylinders[1].y = 0.4;

	cylinders[2].color = "blue";
	cylinders[2].x = -0.5;
	cylinders[2].y = -1.1;

	cylinders[3].color = "green";
	cylinders[3].x = 0.7;
	cylinders[3].y = -2.1;

  ros::Rate loop_rate(3);
  while (ros::ok())
  {
		publish_waypoints();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return (0);
}

