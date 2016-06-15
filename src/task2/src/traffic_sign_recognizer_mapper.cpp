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

void callback(const detection_msgs::Detection& msg)
{
  ROS_INFO("Traffic sign detected with confidence %f", msg.confidence);
}

int main(int argc, char** argv){
	ros::init(argc, argv, "traffic_sign_recognizer_mapper");
	ros::NodeHandle n;

	sleep(1);

	ros::Subscriber sub = n.subscribe("/detector/traffic_signs", -1, callback);   

  ros::spin();
  return 0;
}

