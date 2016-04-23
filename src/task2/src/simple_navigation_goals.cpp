#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

double coords[][2] = {{0.25, 0.15}, {2.0, -0.5}, {3.0, 0.0}, {5.0, 0.0}, {3.75, 2.0}};

//we'll send a goal to the robot to move 1 meter forward
void send_data(int i) {
MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = coords[i][0];
  goal.target_pose.pose.position.y = coords[i][1];

  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved 1 meter forward");
  else
    ROS_INFO("The base failed to move forward 1 meter for some reason");
}

int main(int argc, char** argv){
  	ros::init(argc, argv, "simple_navigation_goals");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("", 1000, faceMapperCallback); 

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

	for(int i = 0; i < 5; i++)
	{
		send_data(i);
	}
  return 0;
}

