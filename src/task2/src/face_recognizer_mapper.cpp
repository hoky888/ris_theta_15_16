
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

struct face
{
	std::string name;
	double x,y;
} faces[8];

ros::Publisher _pub;

void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	int i = 0;
	for(i = 0; i < 8; i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "base_link";
		marker.header.stamp = ros::Time();
		marker.ns = "recognized_faces_markers";
		marker.id = i;
		marker.type = visualization_msgs::Marker::SPHERE;
		marker.action = visualization_msgs::Marker::ADD;
		marker.text = faces[i].name;
		marker.pose.position.x = faces[i].x;
		marker.pose.position.y = faces[i].y;
		marker.pose.position.z = 0.2;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.5;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		marker.scale.x = 0.2;
		marker.scale.y = 0.2;
		marker.scale.z = 0.2;

		marker.color.a = 1.0; // Don't forget to set the alpha!
		marker.color.r = 0.6;
		marker.color.g = 0;
		marker.color.b = 1;
		marker_array.markers.push_back(marker);
	}
	_pub.publish( marker_array );
}

int main(int argc, char** argv){
	ros::init(argc, argv, "face_recognizet_mapper");
	ros::NodeHandle n;
	_pub = n.advertise<visualization_msgs::MarkerArray>("recognized_faces", 1000);
		
	faces[0].name = "harry";
	faces[0].x = -0.45;
	faces[0].y = -1;

	faces[1].name = "scarlett";
	faces[1].x = 2.3;
	faces[1].y = -0.15;

	faces[2].name = "tina";
	faces[2].x = 0.8;
	faces[2].y = -2.1;

	faces[3].name = "pater";
	faces[3].x = 1.4;
	faces[3].y = 0;

	faces[4].name = "kim";
	faces[4].x = 0.5;
	faces[4].y = 0;

	faces[5].name = "filip";
	faces[5].x = 1.2;
	faces[5].y = 1.05;

	faces[6].name = "matthew";
	faces[6].x = 1.7;
	faces[6].y = -1.1;

	faces[7].name = "ellen";
	faces[7].x = 6.1;
	faces[7].y = 1.3;

	sleep(1);
	publish_waypoints();

  ros::spin();
  return 0;
}

