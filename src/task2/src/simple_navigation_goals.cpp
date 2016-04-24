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

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

bool first = true;
double coords[][2] = {{0.25, 0.15}, {2.0, -0.5}, {3.0, 0.0}, {5.0, 0.0}, {3.75, 2.0}};
double faces[3][2];
double temp[50][3];
int facesFound = 0;
int facesVisited = 0;
tf::TransformListener *listener;

void move_to(double x, double y)
{	
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending goal");
  ROS_INFO("Goal: [%f, %f]", x, y);
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
	facesVisited++;
    ROS_INFO("Hooray");
  }
  else {
    ROS_INFO("Fail");
  }
}
/*
void locateSelfSpin()
{	
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  //goal.target_pose.pose.position.x = x;
  //goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.w = 0.0;
  //goal.target_pose.pose.orientation.x = 0.0;
  //goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = 1.0;

  ROS_INFO("Trying to spin");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Spin end");
  else
    ROS_INFO("Failed to spin");
}
*/
//we'll send a goal to the robot to move 1 meter forward
void send_data(int i) {
	move_to(coords[i][0], coords[i][1]);
}

void faceMapperCallback(const visualization_msgs::MarkerArray& msg)
{
    tf::StampedTransform transform;
	visualization_msgs::Marker m = msg.markers[0];
	geometry_msgs::PoseStamped pout;
	geometry_msgs::PoseStamped *pin = new geometry_msgs::PoseStamped();
	pin->header = m.header;
	pin->pose = m.pose;
	
    try{		
        (*listener).lookupTransform("/map", "/base_link", ros::Time(0), transform);
		(*listener).transformPose("/map", ros::Time(0), *pin, "/base_link", pout);
		
		double x = (pout.pose.position.x - transform.getOrigin().x())*-1;
		double y = (pout.pose.position.y - transform.getOrigin().y())*-1;
		
		double l = sqrt(pow(x,2)+pow(y,2));
		x /= l;
		y /= l;
		
		x *= 0.7;
		y *= 0.7;
		
		double goal_x = pout.pose.position.x + x;
		double goal_y = pout.pose.position.y + y;
		
		
	    //ROS_INFO("Roomba rot: [%f, %f, %f, %f]", transform.getRotation().x(), transform.getRotation().y(),transform.getRotation().z(), transform.getRotation().w());
		
	    //ROS_INFO("Roomba pos: [%f, %f, %f]", transform.getOrigin().x(), transform.getOrigin().y(),transform.getOrigin().z());
	    //ROS_INFO("Marker pos: [%f, %f, %f]", m.pose.position.x, m.pose.positio/y, m.pose.position.z);
		//ROS_INFO("Marker map pos: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
		
/*		
		if(first)
		{
			first = false;
			faces[facesFound][0]=pout.pose.position.x;
			faces[facesFound][1]=pout.pose.position.y;
			//move_to(goal_x, goal_y);
			facesFound += 1;
			
			ROS_INFO("Marker map pos: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
			
		}
		*/
		
	
		if (pout.pose.position.z < 0.45) {
			bool obstaja = false;
			for(int i = 0; i < facesFound; i++){
				if(pout.pose.position.x < faces[i][0]+0.25 && pout.pose.position.x > faces[i][0]-0.25 && pout.pose.position.y < faces[i][1]+0.25 && pout.pose.position.y > faces[i][1]-0.25 ){
					obstaja=true;
					break;
				}
			}
			if(!obstaja){
				faces[facesFound][0]=pout.pose.position.x;
				faces[facesFound][1]=pout.pose.position.y;
				facesFound+=1;
				ROS_INFO("Saving face: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
				move_to(faces[facesVisited][0], faces[facesVisited][1]);
			}
		}
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
    }
}

int main(int argc, char** argv){
  	ros::init(argc, argv, "simple_navigation_faces");
	listener = new tf::TransformListener();
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/facemapper/markers", 1000, faceMapperCallback); 

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  /*
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  */
  //locateSelfSpin();
  
  //for (int i = 0; i < 5; i++) {
	//move_to(coords[i][0], coords[i][1]);
	//move_to(1.61, 0.627);
  //}
  /*
  if (facesVisited < 3) {
	ROS_INFO("In first if");
	if (facesFound > 0) {
		ROS_INFO("I should move");
		move_to(faces[facesVisited][0], faces[facesVisited][1]);
		ROS_INFO("I should stop moving");
	}
  }
*/
  ros::spin();
  
  return 0;
}

