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
#include <sstream>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

struct face
{
	double x,y,z;
	double goal_x,goal_y;
	int count;
	bool visited;
} faces[25];
int currentPosition = 11;
int situation = 1;
double x_current = 0;
double y_current = 0;
double theta_current = 0;
std::vector<std::string> streets = {"red", "yellow", "green", "blue"};
std::vector<std::string> people = {"harry", "scarlet", "tina", "peter","kim","filip","matthew","ellen"};
double coords[][2] = {{-0.17, -0.963},{0.853, 0.604},{1.95, -0.254},{3.74, 1.9},{5.81, 0.836}, {2.49,2.77},{-0.232, -0.936}, {-0.232, -0.936}};
int goal = 0;
tf::TransformListener *listener;
tf::TransformListener *odom_listener;
ros::Publisher map_waypoints_pub;
MoveBaseClient *ac;
ros::Publisher cmd_vel_pub_;
int faceCount = 0;

int path_coords_length = 15;
double path_coords[][2] = {{5.76,0.82},{4.45,1.6},{3.81,1.91},{3.21,2.43},{2.58,2.78},{2.94,1.01},{1.95,-0.294},{1.44,0.0698},{1.34,-1.22},{0.77,-1.88},{0.307,-1.46},{-0.198,-0.984},{0.252,-0.289},{0.906,0.441},{1.82,1.84}};

typedef int vertex_t;
typedef double weight_t;

int prices[15][15] = {
			{ 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{ 1, 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1, 1, 0,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1, 1, 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1, 1, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1},
			{-1,-1, 1,-1,-1, 0, 1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1, 0, 1, 1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1, 0,-1,-1,-1,-1,-1, 1,-1},
			{-1,-1,-1,-1,-1,-1, 1,-1, 0, 1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1, 1, 0, 1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0, 1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0, 1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0, 1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0, 1},
			{-1,-1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0}};


const weight_t max_weight = std::numeric_limits<double>::infinity();
 
struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};
 
typedef std::vector<std::vector<neighbor> > adjacency_list_t; 
 
void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));
 
    while (!vertex_queue.empty()) 
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());
 
        // Visit each edge exiting u
	const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
	    if (distance_through_u < min_distance[v]) {
	        vertex_queue.erase(std::make_pair(min_distance[v], v));
 
	        min_distance[v] = distance_through_u;
	        previous[v] = u;
	        vertex_queue.insert(std::make_pair(min_distance[v], v)); 
	    } 
        }
    }
} 
 
std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for ( ; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
} 
 
int Dijkstra(int n, int start, int end)
{
    // remember to insert edges both ways for an undirected graph
    adjacency_list_t adjacency_list(n);
    // 0 = a
    for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(prices[i][j] > 0){
				  adjacency_list[i].push_back(neighbor(j, prices[i][j]));
			}
		}
	}
   
    //int start, end;
	//scanf("%d", &start);
	//scanf("%d", &end);
 
    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;
    DijkstraComputePaths(start, adjacency_list, min_distance, previous);   
    std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
    std::cout << "Path : ";
    
    path.pop_front();
	for (std::list<vertex_t>::iterator it=path.begin(); it != path.end(); ++it){
		 int i = (int) * it ;
		 printf("%d ",i);
	}
  std::cout << '\n';
	return path.front();
}

int DijkstraLength(int n, int start, int end)
{   
    adjacency_list_t adjacency_list(n);
    for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(prices[i][j] > 0){
				  adjacency_list[i].push_back(neighbor(j, prices[i][j]));
			}
		}
	}   
    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;
    DijkstraComputePaths(start, adjacency_list, min_distance, previous);   
    std::list<vertex_t> path = DijkstraGetShortestPathTo(end, previous);
        
    path.pop_front();
    int a = 0;
	for (std::list<vertex_t>::iterator it=path.begin(); it != path.end(); ++it){
		 int i = (int) * it ;		
		 a++;
	}
	printf("%d",a);
	return a;
}

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
	ROS_INFO("rotating");
	geometry_msgs::Twist base_cmd;	
	base_cmd.angular.z = 0.45;
	for(int i = 0; i < angle ; i++)
		cmd_vel_pub_.publish(base_cmd);
}

void moveWithDjikstra(int end)
{
	int path_index = currentPosition;
	while (path_index != end) {
		path_index = Dijkstra(15, path_index, end);
		//move_to(path_coords[path_index][0],path_coords[path_index][1]);
		currentPosition = path_index;
	}
}

void traverse_street(int street) {
	int start;
	int end;
	switch (street) {
		case 0: {
			start = 0;
			end = 1;
			break;
		}
		case 1: {
			start = 2;
			end = 4;
			break;
		}
		case 2: {
			start = 5;
			end = 9;
			break;
		}
		case 3: {
			start = 10;
			end = 14;
			break;
		}
		default:
			return;
	}
	int first = DijkstraLength(15,currentPosition,start);
	int second = DijkstraLength(15,currentPosition,end);

	int path_index = currentPosition;

	if(first <= second ){	
		moveWithDjikstra(start);
		for (unsigned int i = start + 1 ; i <= end; i++) {
			moveWithDjikstra(i);
			for(int g = 0; g < 10; g++){
				rotate(200000);
				ros::spinOnce();
			}
		}	
	}
	else{
		moveWithDjikstra(end);
		for (unsigned int i = end -1; i >= start; i--) {
			moveWithDjikstra(i);
		}			
	}
}

unsigned int levenshtein_distance(const std::string& s1, const std::string& s2) 
{
	const std::size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2+1), prevCol(len2+1);
	
	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++)
                        // note that std::min({arg1, arg2, arg3}) works only in C++11,
                        // for C++98 use std::min(std::min(arg1, arg2), arg3)
			//col[j+1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) });
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1), prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
		col.swap(prevCol);
	}
	return prevCol[len2];
}

int findColor(const std::string& s1)
{
	std::string found;
	int min = 999;
	int index = -1;
	istringstream iss(s1);
	do
	{
		string sub;
		iss >> sub;
			
		for(int i = 0; i < streets.size(); i++){	
			int difference = levenshtein_distance(sub, streets[i]);
			if(difference < min){
				min = difference;
				index = i;
			}	
		}
	} while (iss);
	
	return index;
}
		

int findPerson(const std::string& s1)
{
	std::string found;
	int min = 999;
	int index = -1;
	istringstream iss(s1);
	do
	{
		string sub;
		iss >> sub;
			
		for(int i = 0; i < people.size(); i++){	
			int difference = levenshtein_distance(sub, people[i]);
			if(difference < min){
				min = difference;
				index = i;
			}	
		}
	} while (iss);
	
	return index;
}		
void voiceCallback(const std_msgs::String::ConstPtr& msg)
{
	ros::NodeHandle n;
	ROS_INFO("I heard: [%s]", msg->data.c_str());

	sound_play::SoundClient sc(n,"robotsound");

	sleep(1);
	std::string where = msg->data.c_str();
	/*std::transform(where.begin(), where.end(), where.begin(), ::tolower);
	std::replace(where.begin(), where.end(), '-', ' ');
	*/

	int color = findColor(where);

	if (color == 0) {
		traverse_street(0);
		return;
	}

	
	if (color == 1) {
		traverse_street(1);
		return;
	}

	if (color == 2) {
		traverse_street(2);
		return;
	}

	if (color == 3) {
		traverse_street(3);
		return;
	}

	//std::string cepec = "Stephen will take you now to " + where + ". Hawking out!";


	cepec = "Hit me baby one more time!";
	sc.say(cepec);

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
		
		x *= 0.30;
		y *= 0.30;
		
		
		double goal_x = pout.pose.position.x + x;
		double goal_y = pout.pose.position.y + y;
				
	    //ROS_INFO("Roomba rot: [%f, %f, %f, %f]", transform.getRotation().x(), transform.getRotation().y(),transform.getRotation().z(), transform.getRotation().w());
		//ROS_INFO("Roomba pos: [%f, %f, %f]", transform.getOrigin().x(), transform.getOrigin().y(),transform.getOrigin().z());
	    //ROS_INFO("Marker pos: [%f, %f, %f]", m.pose.position.x, m.pose.positio/y, m.pose.position.z);
		//ROS_INFO("Marker map pos: [%f, %f, %f]", pout.pose.position.x, pout.pose.position.y,pout.pose.position.z);
		
		if (pout.pose.position.z < 0.42 && pout.pose.position.z > 0.30) 
		{
			bool found = false;
			//ROS_INFO("Face count: %d", faceCount);
			for(int i = 0; i < faceCount; i++)
			{
				//ROS_INFO("Face: [%f, %f] - Pose: [%f, %f]", faces[i].x, faces[i].y, pout.pose.position.x, pout.pose.position.y);
				double dist = sqrt(pow(pout.pose.position.x-faces[i].x, 2)+pow(pout.pose.position.y-faces[i].y,2));
				//ROS_INFO("		Dist %d: %f", i, dist);
				if(dist < 0.6)
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
				faces[faceCount].z = pout.pose.position.z;
				faces[faceCount].goal_x = goal_x;
				faces[faceCount].goal_y = goal_y;
				faces[faceCount].count = 1;		
				ROS_INFO("	New face %d : [%f, %f, %f, %f]", faceCount, faces[faceCount].x, faces[faceCount].y, faces[faceCount].goal_x, faces[faceCount].goal_y);

				ROS_INFO("	New face %d : [%f, %f, %f]", faceCount, faces[faceCount].x, faces[faceCount].y, faces[faceCount].z);
				faceCount++;		
			}
		}
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
    }
}


void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	int i = 0;
	for(i = 0; i < path_coords_length ; i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "base_link";
		marker.header.stamp = ros::Time();
		marker.ns = "waypoints";
		marker.id = i;
		marker.type = visualization_msgs::Marker::SPHERE;
		marker.action = visualization_msgs::Marker::ADD;
		marker.pose.position.x = path_coords[i][0];
		marker.pose.position.y = path_coords[i][1];
		marker.pose.position.z = 0;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		marker.scale.x = 0.1;
		marker.scale.y = 0.1;
		marker.scale.z = 0;

		marker.color.a = 1.0; // Don't forget to set the alpha!
		switch(i)
		{
			case 0:
			case 1:
				marker.color.r = 1;
				marker.color.g = 0;
				marker.color.b = 0;
				break;
			case 2:
			case 3:
			case 4:
				marker.color.r = 1;
				marker.color.g = 1;
				marker.color.b = 0;
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				marker.color.r = 0;
				marker.color.g = 1;
				marker.color.b = 0;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				marker.color.r = 0;
				marker.color.g = 0;
				marker.color.b = 1;
				break;
			default:
				marker.color.r = 1;
				marker.color.g = 1;
				marker.color.b = 1;
				break;
		}
		marker_array.markers.push_back(marker);
	}
	map_waypoints_pub.publish( marker_array );
}

int main(int argc, char** argv){
  	ros::init(argc, argv, "simpe_navigation_goals");
	listener = new tf::TransformListener();
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/facemapper/markers", -1, faceMapperCallback);        
	ros::Publisher faces_pub = n.advertise<visualization_msgs::MarkerArray>("detected_faces", 1000);
	map_waypoints_pub = n.advertise<visualization_msgs::MarkerArray>("map_waypoints", 1000);
	cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1);
	odom_listener = new tf::TransformListener();
	
	ros::Subscriber sub1 = n.subscribe("command", -1, voiceCallback);
	sound_play::SoundClient sc(n,"robotsound");
	
  //tell the action client that we want to spin a thread by default
  ac = new MoveBaseClient("move_base", true);
  
 /* while(!ac->waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }*/  
	
	sleep(1);
	sc.say("Greetings friend. Please state your destination.");
	publish_waypoints();

	// Dijkstra
	int path_index = 13;
	//while (path_index != 6) {
	//	path_index = Dijkstra(15, path_index, 6);
	//	move_to(path_coords[path_index][0],path_coords[path_index][1]);
	//}

 /*
	bringup_minimal
	task 2
	map_Server
	rviz
	speech_proxy
	sound_play soundplay_node.launch

*/	
	
  ros::spin();
  return 0;
}
