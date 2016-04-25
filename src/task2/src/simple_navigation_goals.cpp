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
ros::Publisher cmd_VelPublisher;
MoveBaseClient *ac;
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

void get_odom()
{
    tf::StampedTransform transform;
    try
    {
        odom_listener->lookupTransform("/odom", "/base_footprint", ros::Time(0), transform);

        x_current = transform.getOrigin().x();
        y_current = transform.getOrigin().y();
        theta_current = angles::normalize_angle_positive(tf::getYaw(transform.getRotation()));
        //ROS_INFO("odom (x, y, theta) : (%f, %f, %f)", x_current, y_current, theta_current);
    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("%s",ex.what());
    }
}

void turn(double turn_angle)
{
	double angularSpeed = 25;
	int direction = 10;
    // Initialize the movement command message
    geometry_msgs::Twist cmd_velMsg;
    //Set the movement command rotation speed
    cmd_velMsg.angular.z = direction * angularSpeed;

    // How fast to update the robot's movement?
    // Set the equivalent ROS rate variable
    ros::Rate rate(10.0);

    // Current angle
    get_odom();
    double last_angle = theta_current;
    double angle = 0;

     while ((angle < turn_angle) && ros::ok())
    {
        //Publish the Twist message and sleep 1 cycle
        cmd_VelPublisher.publish(cmd_velMsg);

        //ROS_INFO("current_theta: %f, angle + turn_angle: %f", angle, (angle + turn_angle));

        ros::spinOnce();
        rate.sleep();

        // Get the current odometry
        get_odom();

        // Compute the amount of rotation since the last loop
        angle += angles::normalize_angle(theta_current - last_angle);
        last_angle = theta_current;

        //ROS_INFO("angle: %f", angle);
    }

    // Stop turning the robot
    cmd_velMsg.angular.z = 0;
    cmd_VelPublisher.publish(cmd_velMsg);
}

void rotate(int angle)
{	
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.orientation.z = angle;
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending rotation");
  ac->sendGoal(goal);
  ac->waitForResult();
  if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("Hooray rotation");
  }
  else 
  {
    ROS_INFO("Fail rotation");
  }
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
		
		if (pout.pose.position.z < 0.45) 
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
					//ROS_INFO("Existing face: %d - [%f, %f] - %d", i, faces[i].x, faces[i].y, faces[i].count);					
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
				ROS_INFO("New face: [%f, %f]", faces[faceCount].x, faces[faceCount].y);
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
	cmd_VelPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    odom_listener = new tf::TransformListener();
	
  //tell the action client that we want to spin a thread by default
  ac = new MoveBaseClient("move_base", true);
  
  while(!ac->waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  
  for(int i = 0 ; i < 4 ; i++)
  {
	  ROS_INFO("Moving to next goal");
	  move_to(coords[i][0], coords[i][1]); 
	  ROS_INFO("Move to next goal finnished");
	  ros::spinOnce();
	  
	  ROS_INFO("Spin 360");
	  //turn(360);
	  ROS_INFO("Spin finnished");
	  ros::spinOnce();
	  
	  ROS_INFO("Checking for new faces goals");
	  for(int i = 0; i < faceCount; i++)
	  {
		  if(!faces[i].visited && faces[i].count > 5)
			{
				/*
				visualization_msgs::Marker marker;
				marker.header.frame_id = "map";
				marker.header.stamp = ros::Time();
				marker.ns = "my_namespace";
				marker.id = 0;
				marker.type = visualization_msgs::Marker::SPHERE;
				marker.action = visualization_msgs::Marker::ADD;
				marker.pose.position.x = 1;
				marker.pose.position.y = 1;
				marker.pose.position.z = 1;
				marker.pose.orientation.x = faces[i].x;
				marker.pose.orientation.y = faces[i].y;
				marker.pose.orientation.z = 0.0;
				marker.pose.orientation.w = 1.0;
				marker.scale.x = 1;
				marker.scale.y = 0.1;
				marker.scale.z = 0.1;
				marker.color.a = 1.0; // Don't forget to set the alpha!
				marker.color.r = 0.0;
				marker.color.g = 1.0;
				marker.color.b = 0.0;

				ma.markers.push_back(marker);*/
				faces[i].visited = true;
				ROS_INFO("MOVING TO FACE");
				move_to(faces[i].goal_x,faces[i].goal_y,faces[i].x,faces[i].y);
			}
	  }
	  ros::spinOnce();
      faces_pub.publish(ma);
	  ros::spinOnce();
  }
   
  ros::spin();
  
  return 0;
}

