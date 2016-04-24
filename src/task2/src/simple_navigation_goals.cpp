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
} faces[10], visited[10];

double x_current = 0;
double y_current = 0;
double theta_current = 0;

double coords[][2] = {{2.0, -0.309},{2.45, 1.25},{3.9,0.381}, {4.34,2.07}, {5.3,0.554}, {5.91,-0.964}, {1.0, 0.63}};
int goal = 0;
tf::TransformListener *listener;
tf::TransformListener *odom_listener;
ros::Publisher cmd_VelPublisher;
int faceCount = 0;
int visitedCount = 0;

void move_to(double x, double y, double ox, double oy)
{	
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.z = tan((y-oy)/(ox-x));
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending goal");
  ROS_INFO("Goal: [%f, %f]", x, y);
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("Hooray");
  }
  else 
  {
    ROS_INFO("Fail");
  }
}

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

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("Hooray");
  }
  else 
  {
    ROS_INFO("Fail");
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
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.orientation.z = angle;
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("Sending rotation");
  ac.sendGoal(goal);
  ac.waitForResult();
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) 
  {
    ROS_INFO("Hooray rotation");
  }
  else 
  {
    ROS_INFO("Fail rotation");
  }
}

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
		
		double goal_x = pout.pose.position.x + x;
		double goal_y = pout.pose.position.y + y;
				
	    //ROS_INFO("Roomba rot: [%f, %f, %f, %f]", transform.getRotation().x(), transform.getRotation().y(),transform.getRotation().z(), transform.getRotation().w());
		//ROS_INFO("Roomba pos: [%f, %f, %f]", transform.getOrigin().x(), transform.getOrigin().y(),transform.getOrigin().z());
	    //ROS_INFO("Marker pos: [%f, %f, %f]", m.pose.position.x, m.pose.positio/y, m.pose.position.z);
		//ROS_INFO("Marker map pos: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
		
		if (pout.pose.position.z < 0.45) 
		{
			bool found = false;
			for(int i = 0; i < faceCount; i++)
			{
				double dist = sqrt(pow(pout.pose.position.x-faces[i].x, 2)+pow(pout.pose.position.y-faces[i].y,2));
				if(dist < 0.5)
				{
					found = true;
					faces[i].count++;				
					ROS_INFO("Existing face: %d - [%f, %f] - %d", i, faces[i].x, faces[i].y, faces[i].count);
					if(faces[i].count > 10 && !faces[i].visited)
					{
						bool v = true;
						for(int j = 0; j < visitedCount; j++)
						{
							double d2 = sqrt(pow(faces[i].x-visited[j].x, 2)+pow(faces[i].y-visited[j].y,2));
							if(d2 < 0.5)
							{
								v = false;
								break;
							}
						}
						if(v)
						{
							faces[i].visited = true;
							visitedCount++;
							visited[visitedCount].x = faces[i].x;
							visited[visitedCount].y = faces[i].y;
							move_to(faces[i].goal_x,faces[i].goal_y,faces[i].x,faces[i].y);
							ros::Duration(1.5).sleep();
						}
					}
					else
					{
						if(goal < 7)
						{		
							ROS_INFO("Sending waypoint");
							send_data(goal++);
							turn(360);
						}
					}
					break;
				}
			}
			if(!found)
			{
				faceCount++;
				faces[faceCount].x = pout.pose.position.x;
				faces[faceCount].y = pout.pose.position.y;
				faces[faceCount].goal_x = goal_x;
				faces[faceCount].goal_y = goal_y;
				faces[faceCount].count = 1;				
				ROS_INFO("New face: [%f, %f]", faces[faceCount].x, faces[faceCount].y);
				move_to(faces[faceCount].x, faces[faceCount].y);
			} else {
				ROS_INFO("Sending waypoint");
				send_data(goal++);
			}
		}
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
    }
}

int main(int argc, char** argv){
  	ros::init(argc, argv, "waypoin");
	listener = new tf::TransformListener();
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/facemapper/markers", 1, faceMapperCallback);  
	cmd_VelPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    odom_listener = new tf::TransformListener();
	
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);
   
  ros::spin();
  
  return 0;
}

