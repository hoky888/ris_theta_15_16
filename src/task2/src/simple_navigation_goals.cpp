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

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct face
{
	double x,y;
	double goal_x,goal_y;
	int count;
	bool visited;
} faces[10];

double x_current = 0;
double y_current = 0;
double theta_current = 0;
visualization_msgs::MarkerArray ma;
double coords[][2] = {{0.5, 0.2},{2.2, 0.4},{4.0, 1.3}, {6.0, -1.0}};
int goal = 0;
tf::TransformListener *listener;
tf::TransformListener *odom_listener;
MoveBaseClient *ac;
ros::Publisher cmd_vel_pub_;
int faceCount = 0;

void move_to(double x, double y, double ox, double oy)
{	
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.z = tan((y-oy)/(ox-x));
  goal.target_pose.pose.orientation.w = 1.0;

  //ROS_INFO("Sending goal");
  ROS_INFO("	Goal: [%f, %f]", x, y);
  ac->sendGoal(goal);

  ac->waitForResult();

  if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("		SUCCESS");
  }
  else 
  {
    ROS_INFO("		FAIL");
  }
}

void move_to(double x, double y)
{	
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("	Goal: [%f, %f]", x, y);
  ac->sendGoal(goal);

  ac->waitForResult();

  if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("		SUCCESS");
  }
  else 
  {
    ROS_INFO("		FAIL");
  }
}

void rotate(int angle)
{	
	geometry_msgs::Twist base_cmd;	
	base_cmd.angular.z = 0.75;
	for(int i = 0; i < angle ; i++)
		cmd_vel_pub_.publish(base_cmd);
}


void faceMapperCallback(const visualization_msgs::MarkerArray& msg)
{
	//ROS_INFO("msg received");
    tf::StampedTransform transform;
	visualization_msgs::Marker m = msg.markers[0];
	geometry_msgs::PoseStamped pout;
	geometry_msgs::PoseStamped *pin = new geometry_msgs::PoseStamped();
	pin->header = m.header;
	pin->pose = m.pose;
	
    try
	{		
        (*listener).lookupTransform("/map", "/base_link", ros::Time(0), transform);
		(*listener).transformPose("/map", ros::Time(0), *pin, "/base_link", pout);
		
		double x = (pout.pose.position.x - transform.getOrigin().x())*-1;
		double y = (pout.pose.position.y - transform.getOrigin().y())*-1;
		
		double l = sqrt(pow(x,2)+pow(y,2));
		x /= l;
		y /= l;
		
		x *= 0.35;
		y *= 0.35;
		
		x = ((int)round(x*1000))/1000.0;
		y = ((int)round(y*1000))/1000.0;
		
		double goal_x = ((int)round((pout.pose.position.x + x)*1000))/1000.0;
		double goal_y = ((int)round((pout.pose.position.y + y)*1000))/1000.0;
				
	    //ROS_INFO("Roomba rot: [%f, %f, %f, %f]", transform.getRotation().x(), transform.getRotation().y(),transform.getRotation().z(), transform.getRotation().w());
		//ROS_INFO("Roomba pos: [%f, %f, %f]", transform.getOrigin().x(), transform.getOrigin().y(),transform.getOrigin().z());
	    //ROS_INFO("Marker pos: [%f, %f, %f]", m.pose.position.x, m.pose.positio/y, m.pose.position.z);
		//ROS_INFO("Marker map pos: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
		
		if (pout.pose.position.z < 0.35) 
		{
			bool found = false;
			//ROS_INFO("Face count: %d", faceCount);
			for(int i = 0; i < faceCount; i++)
			{
				//ROS_INFO("Face: [%f, %f] - Pose: [%f, %f]", faces[i].x, faces[i].y, pout.pose.position.x, pout.pose.position.y);
				double dist = sqrt(pow(pout.pose.position.x-faces[i].x, 2)+pow(pout.pose.position.y-faces[i].y,2));
				//ROS_INFO("		Dist %d: %f", i, dist);
				if(dist < 0.5)
				{
					found = true;
					faces[i].count++;				
					//ROS_INFO("	Existing face: %d - [%f, %f] - %d", i, faces[i].x, faces[i].y, faces[i].count);					
					break;
				}
			}
			if(!found)
			{
				faces[faceCount].x = pout.pose.position.x;
				faces[faceCount].y = pout.pose.position.y;
				faces[faceCount].goal_x = goal_x;
				faces[faceCount].goal_y = goal_y;
				faces[faceCount].count = 1;		
				ROS_INFO("	New face %d : [%f, %f]", faceCount, faces[faceCount].x, faces[faceCount].y);
				faceCount++;		
			}
		}
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
    }
}

int main(int argc, char** argv){
  	ros::init(argc, argv, "simpe_navigation_goals");
	listener = new tf::TransformListener();
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/facemapper/markers", 1000, faceMapperCallback);  
	ros::Publisher faces_pub = n.advertise<visualization_msgs::MarkerArray>("detected_faces", 1000);
    cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    odom_listener = new tf::TransformListener();
	
  //tell the action client that we want to spin a thread by default
  ac = new MoveBaseClient("move_base", true);
  
  while(!ac->waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  
  for(int i = 0 ; i < 4 ; i++)
  {
	  ROS_INFO("Moving to next goal");
	  //move_to(coords[i][0], coords[i][1]); 
	  ros::spinOnce();
	  
	  rotate(360);
	  ros::spinOnce();
	  /*
	  for(int i = 0; i < faceCount; i++)
	  {
		 ROS_INFO("Detected face %d - visited: %s - count: %d", i,faces[i].visited ? "true" : "false",faces[i].count);
		  if(!faces[i].visited && faces[i].count > 5)
			{
				faces[i].visited = true;
				ROS_INFO("MOVING TO FACE %d", i);
				move_to(faces[i].goal_x,faces[i].goal_y,faces[i].x,faces[i].y);
			}
	  }*/
	  ros::spinOnce();
  }
   
  ros::spin();
  
  return 0;
}

