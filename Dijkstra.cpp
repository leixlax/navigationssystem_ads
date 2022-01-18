// Der Algorithmus von Dijkstra
// K6dijkstra.cpp


//define not directly connected nodes as 10000 so dijkstra does not misunderstand it as a "short path"
//should be much greater than greatest distance in matrix
#define xxx 10000

#include "Dijkstra.h"

//declare array of strings containing city names
string cities[ANZAHL] = {
	"Karlsruhe",      //1
	"Stuttgart",      //2
	"Mannheim",       //3
	"Pforzheim",      //4
	"Rastatt",        //5
	"Baden-Baden",    //6
	"Freiburg",       //7
	"Offenburg",      //8
    "Kehl",           //9
	"Freudenstadt",   //10
	"Reutlingen",     //11
	"Tübingen",       //12
	"Ulm",            //13
	"Lörrach",        //14
	"Konstanz"        //15
};


//adjacency matrix of the graph
const unsigned int adistance[ANZAHL][ANZAHL] ={ 
  // 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15 
    0  ,xxx,71 ,30 ,27 ,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx, //1
    xxx,0  ,133,49 ,xxx,xxx,xxx,xxx,xxx,81 ,40 ,44 ,93 ,xxx,xxx, //2
    71 ,133,0  ,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx, //3
    30 ,49 ,xxx,0  ,xxx,xxx,xxx,xxx,xxx,65 ,xxx,xxx,xxx,xxx,xxx, //4
    27 ,xxx,xxx,xxx,0  ,14 ,xxx,55 ,54 ,65 ,xxx,xxx,xxx,xxx,xxx, //5
    xxx,xxx,xxx,xxx,14 ,0  ,xxx,52 ,xxx,55 ,xxx,xxx,xxx,xxx,xxx, //6
    xxx,xxx,xxx,xxx,xxx,xxx,0  ,67 ,xxx,94 ,xxx,xxx,xxx,73 ,125, //7
    xxx,xxx,xxx,xxx,55 ,52 ,67 ,0  ,18 ,50 ,xxx,xxx,xxx,xxx,xxx, //8
    xxx,xxx,xxx,xxx,54 ,xxx,xxx,18 ,0  ,xxx,xxx,xxx,xxx,xxx,xxx, //9
    xxx,81 ,xxx,65 ,65 ,55 ,94 ,50 ,xxx,0  ,xxx,61 ,xxx,xxx,xxx, //10
    xxx,40 ,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,0  ,15 ,73 ,xxx,129, //11
    xxx,44 ,xxx,xxx,xxx,xxx,xxx,xxx,xxx,61 ,15 ,0  ,xxx,xxx,128, //12
    xxx,93 ,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,73 ,xxx,0  ,xxx,125, //13
    xxx,xxx,xxx,xxx,xxx,xxx,73 ,xxx,xxx,xxx,xxx,xxx,xxx,0  ,142, //14
    xxx,xxx,xxx,xxx,xxx,xxx,125,xxx,xxx,xxx,129,128,125,142,  0  //15
};

//initialize class 
class CNodeinfo{
public:
	int distance;
	int predecessor;
	char done;
	CNodeinfo() {};
};

//declare class name as n_info for an amount of ANZAHL
CNodeinfo n_info[ANZAHL];

//print path with end node -> Needed for first part of 3 point route and for 2 point route
void print_path2(int i)
{	
	//recursion of print_path -> if function has found a city with no predecessor, print starting point and jump to stack adress of previous function  call until done
	if (n_info[i].predecessor != -1) {
		print_path2(n_info[i].predecessor); 
		//print path
		cout << " ->" <<"("<<adistance[i][n_info[i].predecessor]<<"km)-> "; 
		cout << cities[i];
	}
	else{
		cout << cities[i]; //print starting point
	}
		
}
//print path without end node -> needed for second part of 3 point route
void print_path(int i)
{	
	//recursion of print_path -> if function has found a city with no predecessor, print starting point and jump to stack adress of previous function call until done
	if (n_info[i].predecessor != -1) {
		print_path(n_info[i].predecessor);
		
		cout << " ->" <<"("<<adistance[i][n_info[i].predecessor]<<"km)-> "; 
		cout << cities[i];
	}
		
}

//writing distance from starting point to every node from the graph into the n_info class
//mark starting point as done initalizing and having no predecessor 
void init(int start_n){
	for (int i = 0; i < ANZAHL; i++) {
		n_info[i].done = 0; //mark nodes as unvisited
		n_info[i].distance = adistance[start_n][i]; //write distance from adjacency matrix to n_info.distance
		n_info[i].predecessor = start_n;  //mark starting point as predecessor
	}
	n_info[start_n].done = 1;  //mark start node as done
	n_info[start_n].predecessor = -1; //start node has no predecessor
}

int node_select(){
	int min = xxx;  //set minimum distance to a high value (at least equal or greater than maximum distance in adjacency matrix)
	int node_minpos = -1; //set node with the least distance to -1 for showing, that there hasn't been a node yet
	for (int i = 0; i < ANZAHL; i++) {
		//sort nodes after distance and return node with the least distance to the predecessor
		if (n_info[i].distance < min && !n_info[i].done) {
			min = n_info[i].distance;
			node_minpos = i;
		}
	}
	return node_minpos;
}


//calculate shortest path for every combination with the starting point
void dijkstra(int start_n){
	init(start_n);
	for (int i = 0; i < ANZAHL - 1; i++) { //ANZAHL-1 because you dont need to calculate minimal distance to the starting point itself
		int node = node_select();  //iterate through nodes and begin with node with the least distance from starting point
		n_info[node].done = 1; //mark visited note as done
		for (int k = 0; k < ANZAHL; k++) {
			// if node was not visited yet, calculate minimum distance to every other node(if there is one),add it to the total distance and mark node as predecessor (????????)
			if (!(n_info[k].done == 1)) {
				int d = n_info[node].distance + adistance[node][k];
				//if dijkstra found a shorter total path, write it into n_info[k].distance and mark the node as predecessor
				if (d < n_info[k].distance) {
					n_info[k].distance = d;
					n_info[k].predecessor = node;
				}
			}
		}
	}
}

//dijkstra with additional node 
void dijkstra3(int start_n,int over_n,int end_n){

	int error=0; //initialize error as zero in case there was no error


	//check if start node,connection node and end node is equal. If yes, error=1 and print error message
	if ((start_n==end_n)&&(end_n==over_n)){
		cout<< "Es konnte keine Route berechnet werden!\nFehler: Startpunkt,Zwischenpunkt und Endpunkt sind identisch!";
		error=1;
	//check if start node, or end node is equal to connection node. If yes, dont print connection node. -> No error though
	}else if ((start_n==over_n)||(end_n==over_n)){
		cout<< "Die kürzeste Route von "<< cities[start_n]<<" nach "<<cities[end_n]<<" ist: \n\n" ;
	//Else print following message:
	}else{
		cout<< "Die kürzeste Route von "<< cities[start_n]<<" über "<<cities[over_n]<<" nach "<<cities[end_n]<<" ist: \n\n";
	}

	//In case there was no error
	if (error == 0){
		dijkstra(start_n); //Dijkstra algorithm for start node ->calculates min distance to all nodes 
		print_path2(over_n); //Only print path to the connection node (first destination)
		int dist1=n_info[over_n].distance; //dist1 is the distance of the first part of the route

		dijkstra(over_n); //Dijkstra algorithm for the connection node
		print_path(end_n); //Only print path to the end node without the connection node because it has already been printed on the first path
		int dist2=n_info[end_n].distance; //dist2 is the distance of the second part of the route

		cout << "\n\nGesamtdistanz: " << dist1+dist2 << "km" << endl; //print total distance
	}
}


//dijkstra with no additional node
void dijkstra2(int start_n,int end_n){

	
	//print error message if start node equals end node
	if (start_n==end_n){
		cout<< "Es konnte keine Route berechnet werden! \nFehler: Startpunkt und Endpunkt sind identisch";
	//else print following message:
	}else{
		cout<< "Die kürzeste Route von "<< cities[start_n]<<" nach "<<cities[end_n]<<" ist: \n\n" ;

		//same as in function above -> only one dijkstra needed 
		dijkstra(start_n);
		print_path2(end_n);
		cout << "\n\nGesamtdistanz: " << n_info[end_n].distance << "km" << endl;
	}
	
}


void dijkstra_input(int start, int end) {

	dijkstra2(start,end);

	//dijkstra3(KARLSRUHE,ULM,LOERRACH);
	
}

