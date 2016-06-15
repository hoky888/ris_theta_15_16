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
int current_handle = -1;
tf::TransformListener *listener;
tf::TransformListener *odom_listener;

struct location
{
	double x_average,y_average;
	double x_sum, y_sum;
	int detection_count;
	int recognition_count;
	std::string name;
	int name_max;
	std::map<std::string, int> counter;
};

std::vector<location> locations;
std::map<std::string, std::string> mapper = {
	{"Ellen_DeGeneres", "ellen"},
	{"Harry_Potter", "harry"},
	{"Peter_Prevec", "peter"},
	{"Filip_Flisar", "filip"},
	{"Kim_Jong-un", "kim"},
	{"Scarlett_Johansson", "scarlett"},
	{"Matthew_McConaughey", "matthew"},
	{"Tina_Maze", "tina"},
	{"Tom_Hanks", "tom"}
};

void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	for(int i = 0; i < locations.size(); i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "map";
		marker.header.stamp = ros::Time();
		marker.ns = "recognized_faces_markers";
		marker.id = i;
		marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
		marker.action = visualization_msgs::Marker::ADD;
		marker.text = locations[i].name;
		marker.pose.position.x = locations[i].x_average;
		marker.pose.position.y = locations[i].y_average;
		marker.pose.position.z = 0.2;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.5;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		marker.scale.x = 0.2;
		marker.scale.y = 0.2;
		marker.scale.z = 0.2;

		marker.color.a = 1.0;
		marker.color.r = 1.0;
		marker.color.g = 1.0;
		marker.color.b = 0.0;
		marker_array.markers.push_back(marker);
	}
	_pub.publish( marker_array );
}

// Called once when the goal completes
void doneCb(const actionlib::SimpleClientGoalState& state, const face_recognition::FaceRecognitionResultConstPtr& result)
{
	// ta se klice ko bo razpoznal faco
  //ROS_INFO("Goal [%i] Finished in state [%s]", result->order_id,state.toString().c_str());
  if(state.toString() == "SUCCEEDED") 		
	{
		if(result->order_id==0)
		{
			// kar tukaj naredimo je da poiščem pravo ji rečem da je valid, in publishamo nov array, tu lahko še malo predebatiramo
			std::string file_name = result->names[0];
  		std::map<std::string, std::string>::iterator it = mapper.find(file_name);
			if (it != mapper.end())
			{
		 	 	ROS_INFO("%s with confidence %f", file_name.c_str(), result->confidence[0]);  
				if(result->confidence[0] > 0.95)
				{		
					locations[current_handle].counter[file_name]++;
					int count = locations[current_handle].counter[file_name];
	 
					if(count > 3 && count > locations[current_handle].name_max)
					{
						locations[current_handle].name_max = count;
						std::string name = mapper[file_name];
						locations[current_handle].name = name;
						publish_waypoints();
					}			
				}
			}
		}    
	}

	current_handle = -1;    
}

// Called once when the goal becomes active
void activeCb()
{
	// to se bo klicalo ko bo zacel prepoznavat faco
  //ROS_INFO("Goal just went active");
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

	geometry_msgs::PoseStamped pout;
	geometry_msgs::PoseStamped *pin = new geometry_msgs::PoseStamped();
	pin->header = m.header;
	pin->pose = m.pose;

  try
	{		
		(*listener).transformPose("/map", ros::Time(0), *pin, "/base_link", pout);

		x = pout.pose.position.x;
		y = pout.pose.position.y;
  }
  catch (tf::TransformException ex){
    //ROS_ERROR("%s",ex.what());
		current_handle = -1;	
		return;
  }


	int i;
	int index = -1;
	double min_diff = 0;
	for(i = 0; i < locations.size(); i++)
	{
		double diff = sqrt(pow(x-locations[i].x_average, 2)+pow(y-locations[i].y_average, 2));		
		if(diff < 0.4)
		{
			index = i;
			min_diff = diff;
			break;
		}
	}

	if(index >= 0)
	{   
		locations[index].detection_count++;
		locations[index].x_sum += x;
		locations[index].y_sum += y;
		locations[index].x_average = locations[index].x_sum/locations[index].detection_count;
		locations[index].y_average = locations[index].y_sum/locations[index].detection_count;
	}
	else
	{
		location l;		
		l.x_sum = x;
		l.y_sum = y;
		l.detection_count = 1;
		l.x_average = x;
		l.y_average = y;	
		l.name_max = 0;	
		locations.push_back(l);
		index = locations.size()-1;
	}
	
	//to vrstico bi se dalo vrjetno zakomentirat, za premislit!
	if(locations[index].detection_count > 5)
	{
		// če je count > 10; prožim še face recognition, kar bo ta naredil je da bo to kar trenutno vidi poskusil razpoznat
		current_handle = index;
		goal.order_id = 0;
		goal.order_argument = "none";
	 	ac->sendGoal(goal, &doneCb, &activeCb, &feedbackCb);
	}
}

int main(int argc, char** argv){
	ros::init(argc, argv, "face_recognizer_mapper");
	ros::NodeHandle n;

	sleep(1);

	ros::Subscriber sub = n.subscribe("/markers", -1, markersCallback);        
	_pub = n.advertise<visualization_msgs::MarkerArray>("recognized_faces", 10000);   
  ac = new actionlib::SimpleActionClient<face_recognition::FaceRecognitionAction>("face_recognition", true);
	odom_listener = new tf::TransformListener();
	listener = new tf::TransformListener();

  ros::spin();
  return 0;
}

