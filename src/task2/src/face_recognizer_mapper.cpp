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

ros::Publisher _pub;
face_recognition::FaceRecognitionGoal goal; //Goal message
actionlib::SimpleActionClient<face_recognition::FaceRecognitionAction> * ac; //action lib client
const int face_count = 8;
int current_handle = -1;

struct detection
{
	int count;
	double x,y;
	bool set;
} detections[face_count];

struct face
{
	std::string file_name;
	std::string name;
	int recognition_count;
  bool valid;
	double x,y;
} faces[face_count];

int find_face_by_name(std::string name)
{
	for(int i = 0; i < face_count; i++)
	{
		if(faces[i].name == name)
		{
			return i;
		}
	}
	return -1;	
}

int find_face_by_file_name(std::string file_name)
{
	for(int i = 0; i < face_count; i++)
	{
		if(faces[i].file_name == file_name)
		{
			return i;
		}
	}
	return -1;	
}

void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	for(int i = 0; i < face_count; i++)
	{
		if(faces[i].valid == false)
			continue;

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

// Called once when the goal completes
void doneCb(const actionlib::SimpleClientGoalState& state, const face_recognition::FaceRecognitionResultConstPtr& result)
{
	// ta se klice ko bo razpoznal faco
  ROS_INFO("Goal [%i] Finished in state [%s]", result->order_id,state.toString().c_str());
  if(state.toString() != "SUCCEEDED") 
		return;
  if(result->order_id==0)
	{
		// kar tukaj naredimo je da poiščem pravo ji rečem da je valid, in publishamo nov array, tu lahko še malo predebatiramo
    ROS_INFO("%s with confidence %f", result->names[0].c_str(),result->confidence[0]);  
		int index = find_face_by_file_name(result->names[0].c_str());
		faces[index].valid = true;
		publish_waypoints();
		current_handle = -1;
	}        
}

// Called once when the goal becomes active
void activeCb()
{
	// to se bo klicalo ko bo zacel prepoznavat faco
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedbackCb(const face_recognition::FaceRecognitionFeedbackConstPtr& feedback)
{
	// ta se ne bo nikoli prozil, lahko kasneje razlozim
  ROS_INFO("Received feedback from Goal [%d] ", feedback->order_id);
  if(feedback->order_id==1 )
    ROS_INFO("%s with confidence %f", feedback->names[0].c_str(),feedback->confidence[0]);          
}

void markersCallback(const visualization_msgs::MarkerArray& msg)
{
	// iz detektoja dobimo sporočilo, če že obravnavamo eno, ga zavržemo
	if(current_handle != -1)
		return;

	visualization_msgs::Marker m = msg.markers[0];
	double x = m.pose.position.x;
	double y = m.pose.position.y;

	int index = -1;
	int not_set = -1;
	int i;
	// pogledam če že imamo kdaj zaznano to koordinato
	for(i = 0; i < face_count; i++)
	{
		if(detections[i].set == false)
		{
			not_set = i;
			continue;
		}
		double diff = sqrt(pow(x-detections[i].x, 2)+pow(y-detections[i].y, 2));
		if(diff < 0.5)
		{
			index = i;
			break;
		}
	}
	
	if(index >= 0)
	{
		// to pomeni da sem to koordinato že videl, samo count povečam
		detections[i].count++;
		if(detections[i].count > 10)
		{
			// če je count > 10; prožim še face recognition, kar bo ta naredil je da bo to kar trenutno vidi poskusil razpoznat
			current_handle = i;
			goal.order_id = 0;
			goal.order_argument = "none";
		 	ac->sendGoal(goal, &doneCb, &activeCb, &feedbackCb);
		}
	}
	else 
	{
		// te koordinate še nisem videl, nevem kako to lepo rešit
		if(not_set >= 0)
		{
			// dodam koordinato na prvo prosto mesto
			detections[not_set].x = x;
			detections[not_set].y = y;
			detections[not_set].count = 1;
			detections[not_set].set = true;
		}
		else
		{
			// ni več prostih mest, kaj zdaj??? vrjetno imamo nekje false detections
			//TODO
		}
	}
}

int main(int argc, char** argv){
	ros::init(argc, argv, "face_recognizer_mapper");
	ros::NodeHandle n;

	sleep(1);
	ros::Subscriber sub = n.subscribe("/markers", 1, markersCallback);        
	_pub = n.advertise<visualization_msgs::MarkerArray>("recognized_faces", 1000);   

  ac = new actionlib::SimpleActionClient<face_recognition::FaceRecognitionAction>("face_recognition", true);

	for(int i = 0; i < face_count ; i++)
	{
		faces[i].valid = false;
		faces[i].recognition_count = 0;	

		detections[i].set = false;
	}
		
	faces[0].name = "harry";
	faces[0].file_name = "Harry_Potter";
	faces[0].x = -0.45;
	faces[0].y = -1;

	faces[1].name = "scarlett";
	faces[1].file_name = "Scarlett_Johansson";
	faces[1].x = 2.3;
	faces[1].y = -0.15;

	faces[2].name = "tina";
	faces[2].file_name = "Tina_Maze";
	faces[2].x = 0.8;
	faces[2].y = -2.1;

	faces[3].name = "pater";
	faces[3].file_name = "Peter_Prevec";
	faces[3].x = 1.4;
	faces[3].y = 0;

	faces[4].name = "kim";
	faces[4].file_name = "Kim_Jong-un";
	faces[4].x = 0.5;
	faces[4].y = 0;

	faces[5].name = "filip";
	faces[5].file_name = "Filip_Flisar";
	faces[5].x = 1.2;
	faces[5].y = 1.05;

	faces[6].name = "matthew";
	faces[6].file_name = "Matthew_McConaughey";
	faces[6].x = 1.7;
	faces[6].y = -1.1;

	faces[7].name = "ellen";
	faces[7].file_name = "Ellen_DeGeneres";
	faces[7].x = 6.1;
	faces[7].y = 1.3;
/*
  ros::Rate loop_rate(1);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
*/
    ros::spin();
  return 0;
}

