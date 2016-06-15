#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
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

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int currentPosition = 11;
int situation = 1;
int customer = -1;

std::vector<std::string> streets = {"red", "yellow", "green", "blue"};
std::vector<std::string> people = {"harry", "scarlett", "tina", "peter", "kim", "filip", "matthew", "ellen"};

ros::Publisher map_waypoints_pub;
ros::Publisher cmd_vel_pub_;
MoveBaseClient *ac;

double path_coords[][2] = {{5.76,0.82},{4.45,1.6},{3.81,1.91},{3.21,2.43},{2.58,2.78},{2.94,1.01},{1.95,-0.294},{1.44,0.0698},{1.34,-1.22},{0.77,-1.88},{0.307,-1.46},{-0.198,-0.984},{0.252,-0.289},{0.906,0.441},{1.82,1.84}};

visualization_msgs::MarkerArray recognized_faces;

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
  goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.z = tan((y-oy)/(ox-x));
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
	base_cmd.angular.z = 1;
	for(int i = 0; i < angle ; i++)
		cmd_vel_pub_.publish(base_cmd);
}

void moveWithDjikstra(int end)
{
	int path_index = currentPosition;
	while (path_index != end) {
		path_index = Dijkstra(15, path_index, end);
		move_to(path_coords[path_index][0],path_coords[path_index][1]);
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
				rotate(175000);
				ros::spinOnce();
				if(situation == 2)
				return;	
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
	std::istringstream iss(s1);
	do
	{
		std::string sub;
		iss >> sub;
			
		for(int i = 0; i < streets.size(); i++){	
			int difference = levenshtein_distance(sub, streets[i]);
			if(difference < min){
				min = difference;
				index = i;
			}	
		}
	} while (iss);
	
	if(min >= 2)
		index = -1;
	
	return index;
}		

int findPerson(const std::string& s1)
{
	std::string found;
	int min = 999;
	int index = -1;
	std::istringstream iss(s1);
	do
	{
		std::string sub;
		iss >> sub;
			
		for(int i = 0; i < people.size(); i++){	
			int difference = levenshtein_distance(sub, people[i]);
			if(difference < min){
				min = difference;
				index = i;
			}	
		}
	} while (iss);
	
	if(min >= 2)
		index = -1;
	
	printf("%d\n",index);
	return index;
}		

void voiceCallback(const std_msgs::String::ConstPtr& msg)
{
	ros::NodeHandle n;
	ROS_INFO("I heard: [%s]", msg->data.c_str());

	sound_play::SoundClient sc(n,"robotsound");

	sleep(1);
	std::string where = msg->data.c_str();
	std::transform(where.begin(), where.end(), where.begin(), ::tolower);
	std::replace(where.begin(), where.end(), '-', ' ');
	

	int color = findColor(where);
	customer = findPerson(where);
	

	if (color == 0 && customer != -1) {
		printf("%s", "šou na rdeco");
		traverse_street(0);
		return;
	}

	
	if (color == 1 && customer != -1) {
	printf("%s", "šou na rumena");
		traverse_street(1);
		return;
	}

	if (color == 2 && customer != -1) {
		printf("%s", "šou na zeleno");
		traverse_street(2);
		return;
	}

	if (color == 3 && customer != -1) {
		printf("%s", "šou na modro");
		traverse_street(3);
		return;
	}

	//std::string cepec = "Stephen will take you now to " + where + ". Hawking out!";


	std::string cepec = "Nope!";
	sc.say(cepec);

}

void faceRecognizerMapperCallback(const visualization_msgs::MarkerArray& msg)
{
	recognized_faces = msg;
	visualization_msgs::Marker m = msg.markers[0];
}

void publish_waypoints() {
	visualization_msgs::MarkerArray marker_array;
	int i = 0;
	for(i = 0; i < ARRAY_SIZE(path_coords) ; i++)
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "map";
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
		marker.scale.z = 0.1;

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
	ros::NodeHandle n;
	ros::Subscriber sub2 = n.subscribe("recognized_faces", -1, faceRecognizerMapperCallback);        
	map_waypoints_pub = n.advertise<visualization_msgs::MarkerArray>("map_waypoints", 1000);
	cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1);
	
	ros::Subscriber sub1 = n.subscribe("command", -1, voiceCallback);
	sound_play::SoundClient sc(n,"robotsound");
	
  //tell the action client that we want to spin a thread by default
 	ac = new MoveBaseClient("move_base", true);
  
 /* while(!ac->waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }*/  
	
	sleep(1);
	sc.say("Greetings friend. Please state your destination.");
	
	// Dijkstra
	//int path_index = 13;
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

  ros::Rate loop_rate(3);
  while (ros::ok())
  {
		publish_waypoints();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
