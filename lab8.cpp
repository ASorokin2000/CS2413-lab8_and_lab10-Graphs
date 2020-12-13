// ---------------------------------------------------------------------------------------
// AUTHOR:			Aleksei Sorokin
// FILENAME:		lab8.cpp
// SPECIFICATION:	A graph (creating using csv file as an input, printing adjacent matrix 
// 					and finding the shortest distance using Dijkstra’s Algorithm)
// FOR:				CS 2413 Data Structure Section 504
// ---------------------------------------------------------------------------------------

#include"iostream"
#include"cstdlib"
#include<fstream>
#include<alloc.h>
#include<iomanip>
#define MAX_NUM_NODES 20

// structure for Dijkstra’s Algorithm
struct ver{
	int label; // 0 - no connection, 1 - in progress, 2 - the shortest distance to this node is found
	int weight;
	};

struct node{
	char		vertex;
	int			weight;
	struct node *next;
	};

using namespace std;

// --------------------------------------------------------------------------------
// NAME: add_new_vertex - it's the first edge with this vertex
// INPUT Param.: graph - array of linked lists, int current_num - number of lists in the arrat, 
//				 char vertex - a new vertex, char connected_vertex - vertex connected to the new vertex.
//				 int weight - weight of the edge
// OUTPUT Param.: NONE
// PURPOSE: add new linked list to the array // ---------------------------------------------------------------------------------

void add_new_vertex (node *graph[], int *num_elem, char vertex, char connected_vertex, int weight)
{
	node *new_node, *con_node;
	new_node = (struct node*)malloc(sizeof(struct node));
	con_node = (struct node*)malloc(sizeof(struct node));
	new_node->vertex = vertex;
	new_node->weight = 0;
	con_node->vertex = connected_vertex;
	con_node->weight = weight;
	con_node->next = NULL;
	new_node->next = con_node;
	graph[*num_elem] = new_node;
	(*num_elem)++;
}

// --------------------------------------------------------------------------------
// NAME: add_new_edge
// INPUT Param.: node *vertex - a linked list, char connected_vertedx, int weight - weight of the edge
// OUTPUT Param.: node *vertex
// PURPOSE: add new edge to the linked list// ---------------------------------------------------------------------------------

node *add_new_edge (node *vertex, char connected_vertex, int weight)
{
	node *con_node, *current;
	con_node = (struct node*)malloc(sizeof(struct node));
	con_node->vertex = connected_vertex;
	con_node->weight = weight;
	con_node->next = NULL;
	current = vertex;
	while (current->next != NULL)
	{
		current = current->next;
		if (current->vertex == connected_vertex) // I don't allow parallel edges
			return(vertex);
	}
	current->next = con_node;
	return(vertex);
}

// --------------------------------------------------------------------------------
// NAME: create
// INPUT Param.: graph - array of linked lists, int current_num - number of lists in the arrat, 
// OUTPUT Param.: NONE
// PURPOSE: create a graph using csv file as an input // ---------------------------------------------------------------------------------

void create (node *graph[], int *num_elem)
{
	int  weight;
	char first_vertex, second_vertex;
	ifstream myFile;
	string line;
	myFile.open("input.csv");
	while(myFile.good()){ 
		string line;
		getline(myFile, line, ',');
		first_vertex = line[line.length()-1]; 
		getline(myFile, line, ',');
		second_vertex = line[line.length()-1];
		getline(myFile, line, '\n');
		weight = 0;
		int i = 0;
		if (line[i] < '0' || line[i] > '9')
			break; // the program reads last empty string from the file, I don't need it.
		// converting from string to integer
		while (i < line.length() && line[i] >= '0' && line[i] <= '9')
		{
			weight = weight*10 + ((int)line[i]) - 48;
			i++;
		}
		//
		int exist = 0, index_first, index_second; // 0 - both vertices are not in the array, 1 - first is in the array, 2 - second is in the array, 3 - both are in the array
		for (int j = 0; j < *num_elem; j++)
		{
			if (first_vertex == graph[j]->vertex)
			{
				exist++;
				index_first = j;
			}
			if (second_vertex == graph[j]->vertex)
			{
				exist += 2;
				index_second = j;
			}
			if (exist == 3)
				break;
		}
		if (exist == 0)
		{
			add_new_vertex (graph, num_elem,  first_vertex, second_vertex, weight);
			add_new_vertex (graph, num_elem, second_vertex, first_vertex, weight);
		}
		if (exist == 1)
		{
			add_new_edge (graph[index_first], second_vertex, weight);
			add_new_vertex (graph, num_elem, second_vertex, first_vertex, weight);
		}
		if (exist == 2)
		{
			
			add_new_edge (graph[index_second], first_vertex, weight);
			add_new_vertex (graph, num_elem, first_vertex, second_vertex, weight);
		}
		if (exist == 3)
		{
			add_new_edge (graph[index_second], first_vertex, weight);
			add_new_edge (graph[index_first], second_vertex, weight);
		}
		
	}
	myFile.close();
}

// --------------------------------------------------------------------------------
// NAME: display
// INPUT Param.: graph - array of linked lists, int current_num - number of lists in the arrat, 
// OUTPUT Param.: NONE
// PURPOSE: display the adjacent matrix // ---------------------------------------------------------------------------------

void display (node *graph[], int num_elem)
{
	node *current;
	cout << endl << " ";
	for (int i = 0; i < num_elem; i++)
		cout << setw(5) << graph[i]->vertex;
	cout << endl;
	for (int i = 0; i < num_elem; i++)
	{
		cout << graph[i]->vertex;
		for(int j = 0; j < num_elem; j++)
		{
			int connection = 0, weight;
			current = graph[i]->next;
			// look if there is a connection or not
			while(current)
				if (current->vertex == graph[j]->vertex)
				{
					connection = 1;
					weight = current->weight;
					break;
				}
				else current = current->next;
			//
			if (connection)
			cout << setw(5) << weight;
			else cout << setw(5) << 0 ;
		}
		cout << endl;
	}
}

// --------------------------------------------------------------------------------
// NAME: shortest_distance
// INPUT Param.: graph - array of linked lists, int current_num - number of lists in the arrat, 
// OUTPUT Param.: NONE
// PURPOSE: find the shortest distance to all nodes from the source node using Dijkstra’s Algorithm)// ---------------------------------------------------------------------------------

void shortest_distance (node *graph[], int num_elem)
{
	char vertex;
	cout << "\nEnter the source node -> ";
	cin >> vertex;
	int exist = 0, current_position;
	node *current;
	for (int i = 0; i < num_elem; i++)
		if (graph[i]->vertex == vertex)
		{
			exist = 1;
			current_position = i;
			break;
		}
	if (!exist)
		cout << "There is no such node in the graph";
	else
	{
		int minimum_position;
		ver set[num_elem];
		for (int i = 0; i < num_elem; i++)
		{
			set[i].label = 0; // initially there are no connections
			set[i].weight = -1;
		}
		set[current_position].label = 2; 
		set[current_position].weight = 0; // the shortest distance to the source node itself is 0 
		do
		{
			int minimum_weight = -1;
			minimum_position = -1;
			current = graph[current_position]->next;
			while(current)
			{
				for (int i = 0; i < num_elem; i++)
					if (current->vertex == graph[i]->vertex) // looking for adjacent nodes
					{
						if (set[i].label != 2)
						{
							if (set[i].weight == -1 || set[i].weight > set[current_position].weight + current->weight)
								set[i].weight = set[current_position].weight + current->weight;
							if (set[i].label == 0)
								set[i].label = 1;
						}
						break;	
					}
				current = current->next;
			}
			// defining the minimum distance node
			for (int i = 0; i < num_elem; i++)
				if (set[i].label == 1 && (minimum_position == -1 || set[i].weight < minimum_weight))
				{
					minimum_position = i;
					minimum_weight = set[i].weight;
				}
			if (minimum_position != -1)
			{
				set[minimum_position].label = 2;
				current_position = minimum_position;
			}
		}
		while(minimum_position != -1); // if minimum_position == 1, it means that no nodes in process and no adjacent nodes to the current node.
		cout << "Source node\t Destination node\t Shortest distance\n";
		for (int i = 0; i < num_elem; i++)
			if (set[i].label == 2)
				cout << setw(5) << vertex << setw(20) << graph[i]->vertex << setw(20) << set[i].weight << endl;
			else // label = 0, no connection
				cout << setw(5) << vertex << setw(20) << graph[i]->vertex << "\t\t No connection between these nodes\n";
	}
}

int main()
{
	char key = ' ';
	node *graph[MAX_NUM_NODES];
	int num_elem = 0;
	cout << "Enter '1' to create a graph by reading information from an input CSV file";
	cout << "\nEnter '2' to show the adjacent matrix associated with weights";
	cout << "\nEnter '3' to get the shortest distance from the input node to all other nodes using Dijkstra’s Algorithm.";
	cout << "\nEnter '0' to exit";
	while(key != '0')
	{
		cin.clear();
		fflush(stdin);
		cout << "\nEnter a number: ";
		cin >> key;
		switch (key)
		{
		case '1': create(graph, &num_elem);
			break;
		case '2': display(graph, num_elem);
			break;
		case '3': shortest_distance(graph, num_elem);
			break;
		case '0': cout << "End of the program";
			break;
		default:  cout << "Wrong input, try again\n";
		}
	}
	return(0);
}
