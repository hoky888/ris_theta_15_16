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

const int SITUATION_SEARCHING_FOR_PERSON = 1;
const int SITUATION_ASK_FOR_GOAL = 2;
const int SITUATION_CONFIRM_GOAL = 3;
const int SITUATION_MOVE_TO_GOAL = 4;

int situation_goal;

const int COLOR_RED = 0;
const int COLOR_YELLOW = 1;
const int COLOR_GREEN = 2;
const int COLOR_BLUE = 3;

int currentPosition = 11;
int situation = 1;
std::string customer = "";

std::vector<std::string> streets = {"red", "yellow", "green", "blue"};
std::vector<std::string> people = {"harry", "scarlett", "tina", "peter", "kim", "filip", "matthew", "ellen"};

ros::Publisher map_waypoints_pub;
ros::Publisher cmd_vel_pub_;
MoveBaseClient *ac;
sound_play::SoundClient *sc;

double path_coords[][2] = {{5.76,0.82},{4.45,1.6},{3.81,1.91},{3.21,2.43},{2.58,2.78},{2.94,1.01},{1.95,-0.294},{1.44,0.0698},{1.34,-1.22},{0.77,-1.88},{0.307,-1.46},{-0.198,-0.984},{0.252,-0.289},{0.906,0.441},{1.92,1.67}};

visualization_msgs::MarkerArray recognized_faces;
visualization_msgs::MarkerArray recognized_cylinders;
visualization_msgs::MarkerArray recognized_traffic_signs;

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

struct line
{
	double a,b,c;	
	double x,y;
} borders[6];

int find_nearest_line(double x, double y)
{
	int index = -1;
	double min = 999;
	for(int i = 0; i < ARRAY_SIZE(borders) ; i++)
	{
		double diff = abs(borders[i].a*x + borders[i].b*y +  borders[i].c)/sqrt(pow(borders[i].a,2)+pow(borders[i].b,2));
		if(diff < min) 
		{
			min = diff;
			index = i;
		}
	}
	return index;
}

void add_border(int i, double x1, double y1, double x2, double y2, double x, double y)
{
	line l;
	l.a = y1-y2;
	l.b = x2-x1;
	l.c = (x1-x2)*y1 + (y2-y1)*x1;
	l.x = x;
	l.y = y;
	borders[i] = l;	
}
 
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

void rotate()
{	
	geometry_msgs::Twist base_cmd;	
	base_cmd.angular.z = 0.5;
	ros::Rate loop_rate(5);
	for(int i = 0; i < 50 ; i++)
	{
		cmd_vel_pub_.publish(base_cmd);
		loop_rate.sleep();
		ros::spinOnce();
		
		if(!ros::ok())
		{
			break;
		}
	}
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

void traverse_street(int street, bool isRetry) {
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
			rotate();
			
			ROS_INFO("preverjam");
			for(int k = 0; k < recognized_faces.markers.size(); k++){
				ROS_INFO("%d %s %s",k, customer.c_str(), recognized_faces.markers[k].text.c_str());
				if(recognized_faces.markers[k].text.compare(customer) == 0)
				{					
					double x = recognized_faces.markers[k].pose.position.x;
					double y = recognized_faces.markers[k].pose.position.y;
					
					int index = find_nearest_line(x,y);
				
					x += borders[index].x;
					y += borders[index].y;
					
					move_to(x, y);
					
					situation = SITUATION_ASK_FOR_GOAL;
					sc->say("Where can i take you?");
					return;				
				}
			}
		}	
		
		if(!isRetry)
		{
			sc->say("I cant find wanted person, i will retry the search!");		
			traverse_street(street, true);
		}
		else
		{
			sc->say("I still cant find wanted person! Aborting.");	
			situation = SITUATION_SEARCHING_FOR_PERSON;
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

std::string findPerson(const std::string& s1)
{
	int min = 999;
	std::string name;
	std::istringstream iss(s1);
	do
	{
		std::string sub;
		iss >> sub;
			
		for(int i = 0; i < people.size(); i++){	
			int difference = levenshtein_distance(sub, people[i]);
			if(difference < min){
				min = difference;
				name = people[i];
			}	
		}
	} while (iss);
	
	if(min >= 2)
		name = "";
	
	return name;
}		

void move_to_goal()
{
	std::string search = "";
	switch(situation_goal)
	{
		case COLOR_RED:
			search = "red";
			break;
		case COLOR_YELLOW:
			search = "yellow";
			break;
		case COLOR_GREEN:
			search = "green";
			break;
		case COLOR_BLUE:
			search = "blue";
			break;
	}
	
	int i;
	for(i = 0; i < recognized_cylinders.markers.size(); i++)
	{
		if(recognized_cylinders.markers[i].text.compare(search) == 0)
			break;
	}
	
	double x = recognized_cylinders.markers[i].pose.position.x;
	double y = recognized_cylinders.markers[i].pose.position.y;
	
	ROS_INFO("Moving to cylinder location [%f, %f]", x,y);
	
	move_to(x,y);
	sc->say("We arrived at your destination. Get out!!!");	
	situation = SITUATION_SEARCHING_FOR_PERSON;
}

void voiceCallback(const std_msgs::String::ConstPtr& msg)
{
	/*
	ros::NodeHandle n;
	sound_play::SoundClient sc(n,"robotsound");
	sleep(1);
	*/
	
	if(situation == SITUATION_SEARCHING_FOR_PERSON)
	{
		ROS_INFO("I heard: [%s]", msg->data.c_str());

		std::string where = msg->data.c_str();
		std::transform(where.begin(), where.end(), where.begin(), ::tolower);
		std::replace(where.begin(), where.end(), '-', ' ');
		
		//DOBI IME STRANKE IN KAM MORE IT
		int color = findColor(where);
		customer = findPerson(where);

		ROS_INFO("Color: [%d], Person: [%s]", color, customer.c_str());
		
		if(color != -1 && !customer.empty())
		{
			traverse_street(color, false);
		}
		else
		{
			sc->say("Sorry, i didn't understand that.");				
		}		
	}
	else if(situation == SITUATION_ASK_FOR_GOAL)
	{		
		std::string where = msg->data.c_str();
		std::transform(where.begin(), where.end(), where.begin(), ::tolower);
		std::replace(where.begin(), where.end(), '-', ' ');

		situation_goal = findColor(where);
		std::string color_name = "";
		switch(situation_goal)
		{
			case COLOR_RED:
				color_name = "red";
				break;
			case COLOR_YELLOW:
				color_name = "yellow";
				break;
			case COLOR_GREEN:
				color_name = "green";
				break;
			case COLOR_BLUE:
				color_name = "blue";
				break;
		}
		
		sc->say(("You want me to take you to " + color_name + " building?").c_str());
		situation = SITUATION_CONFIRM_GOAL;
	}
	else if(situation == SITUATION_CONFIRM_GOAL)
	{
		std::string where = msg->data.c_str();
		std::transform(where.begin(), where.end(), where.begin(), ::tolower);
		std::replace(where.begin(), where.end(), '-', ' ');
		
		if(where.compare("yes") == 0)
		{
			move_to_goal();
		}
		else if(where.compare("no") == 0)
		{
			situation = SITUATION_ASK_FOR_GOAL;
			sc->say("Sorry, where do you want me to take you?");
		}
		else
		{
			sc->say("Sorry, i didn't understand that.");			
		}
	}
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

void facesCallback(const visualization_msgs::MarkerArray& msg)
{
	recognized_faces = msg;	
}

void cylindersCallback(const visualization_msgs::MarkerArray& msg)
{
	recognized_cylinders = msg;
}

void trafficSignsCallback(const visualization_msgs::MarkerArray& msg)
{
	recognized_traffic_signs = msg;
}

int main(int argc, char** argv){
	ros::init(argc, argv, "simpe_navigation_goals");
	ros::NodeHandle n;

	ros::Subscriber sub1 = n.subscribe("command", -1, voiceCallback);
	ros::Subscriber sub2 = n.subscribe("recognized_faces", -1, facesCallback); 
	ros::Subscriber sub3 = n.subscribe("recognized_cylinders", -1, cylindersCallback); 
	//ros::Subscriber sub4 = n.subscribe("recognized_traffic_signs", -1, trafficSignsCallback);  
      
	map_waypoints_pub = n.advertise<visualization_msgs::MarkerArray>("map_waypoints", 1000);
	cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1);
	
	sc = new sound_play::SoundClient(n,"robotsound");	
 	ac = new MoveBaseClient("move_base", true);
  
	add_border(0, 2.4, 3.6, -0.9, -1.1, 0.15, -0.10);
	add_border(1, -0.9, -1.1, 0.7, -2.6, 0.12, 0.18);
	add_border(2, 0.7, -2.6, 3.8, 1.2, -0.15, 0.10);
	add_border(3, 3.8, 1.2, 6.1, -0.1, 0.12, 0.18);
	add_border(4, 6.1, -0.1, 6.7, 0.8, -0.15, 0.10);
	add_border(5, 6.7, 0.8, 2.4, 3.6, -0.12, -0.18);
  
	sleep(1);
	sc->say("Greetings friend. Please state your destination.");
	

    ros::Rate loop_rate(3);
	while (ros::ok())
	{
		publish_waypoints();
		ros::spinOnce();
		loop_rate.sleep();
    }
	
	return 0;
}
