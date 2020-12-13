// ---------------------------------------------------------------------------------------
// AUTHOR:			Aleksei Sorokin
// FILENAME:		lab10.cpp
// SPECIFICATION:	Kruskal's algorithm to find Minimum Spanning Tree of a given connected, undirected and weighted graph.
// FOR:				CS 2413 Data Structure Section 504
// ---------------------------------------------------------------------------------------

#include"iostream"
#include"cstdlib"
#include<set>
#include<algorithm>
#include<vector>
#define MAX_NUM_EDGES 40

using namespace std;


// structure for the edge of the graph
struct edge{
	char	source; 
	char	destination;
	int 	weight;
	};
	
// --------------------------------------------------------------------------------
// NAME: compare_edge
// INPUT Param.: edge - first, edge - second
// OUTPUT Param.: 1 - first is greater, 0 - second is greater or equal
// PURPOSE: comparing weights of edges// ---------------------------------------------------------------------------------	
	
bool compare_edge( edge first, edge second)
{
    return (first.weight < second.weight);
}	

// --------------------------------------------------------------------------------
// NAME: display
// INPUT Param.: mst - an array of edges, int size_mst - number of elements in mst 
// OUTPUT Param.: NONE
// PURPOSE: print all edges of the Minimum Spanning Tree // ---------------------------------------------------------------------------------

void display( edge mst[], int size_mst)
{
	cout << "\nEdges of MST\n";
	for (int i = 0; i < size_mst; i++)
		cout  << mst[i].source << " " << mst[i].destination << " " << mst[i].weight << "\n";
}

// --------------------------------------------------------------------------------
// NAME: cost
// INPUT Param.: mst - an array of edges, int size_mst - number of elements in mst 
// OUTPUT Param.: NONE
// PURPOSE: calculate the sum of weights of the Minimum Spanning Tree // ---------------------------------------------------------------------------------

void cost( edge mst[], int size_mst)
{
	int total = 0;
	for (int i = 0; i < size_mst; i++)
		total += mst[i].weight;
	cout << "\nTotal cost of the MST = " << total;
}

// --------------------------------------------------------------------------------
// NAME: find
// INPUT Param.: subsets - a vector of sets, chars a, b - vertices. 
// OUTPUT Param.: int *pos_a, int *pos_b 
// PURPOSE: find the subsets of elements a and b // ---------------------------------------------------------------------------------

void find( vector <set <char> > subsets, int *pos_a, int *pos_b, char a, char b)
{
	int j = 0, len = subsets.size();
	*pos_a = *pos_b = -1;
	while (j < len && (*pos_a == -1 || *pos_b == -1))
	{
		if (subsets[j].count(a) == 1)
			*pos_a = j;
		if (subsets[j].count(b) == 1)
			*pos_b = j;
		j++;
	}
}

// --------------------------------------------------------------------------------
// NAME: merge
// INPUT Param.:  chars a, b - vertices, int pos_a, int pos_b . 
// OUTPUT Param.: *subsets - a vector of sets,
// PURPOSE: merge two sets, or add one vertices to the set, or add a new set of two vetices to the vector// ---------------------------------------------------------------------------------

void merge( vector <set <char> > *subsets, int pos_a, int pos_b, char a, char b) 
{
	if (pos_a == -1 && pos_b == -1) // if both vertices were not in the sets, we should create a new set
	{
		set <char> s;
		s.insert(a);
		s.insert(b);
		(*subsets).push_back(s);
	}
	else if (pos_a == -1)
		(*subsets)[pos_b].insert(a);
	else if (pos_b == -1)
		(*subsets)[pos_a].insert(b);
	else // both vertices are already in the MST, then we add all elements of the second set to the first and delete second set. 
	{
		(*subsets)[pos_a].insert((*subsets)[pos_b].begin(), (*subsets)[pos_b].end());
		(*subsets).erase ((*subsets).begin() + pos_b);
	}
}


// --------------------------------------------------------------------------------
// NAME: create
// INPUT Param.: mst - an array of edges, int *size_mst - number of elements in mst 
// OUTPUT Param.: NONE
// PURPOSE: read all edges of the initial graph and find Minimum Spanning Tree using Kruskal's algorithm// ---------------------------------------------------------------------------------
	
void create( edge mst[], int *size_mst)
{
	set <char> vertex; // set of all vertices in the initial graph
	vector <set <char> > subsets;
	edge graph[MAX_NUM_EDGES];
	int num_vertex, num_edges;
	cout << "Enter the number of edges in the initial graph -> ";
	cin >> num_edges;
	cout << "Enter source, destination and weight for " << num_edges << " edges\n";
	for (int i = 0; i < num_edges; i++)
	{
		cin >> graph[i].source >> graph[i].destination >> graph[i].weight;
		vertex.insert(graph[i].source);
		vertex.insert(graph[i].destination);
	}
	num_vertex = vertex.size();
	sort(graph, graph+num_edges, compare_edge); // sort by weights in ascending order
	int i = 0;
	while (i < num_edges && *size_mst != num_vertex - 1)
	{
		char a = graph[i].source;
		char b = graph[i].destination;
		int pos_a, pos_b;
		find(subsets, &pos_a, &pos_b, a, b); // find in which subset are a and b
		if ( !(pos_a == pos_b && pos_a != -1)) // if two elements were in the same subset - it's a cycle
		{
			merge(&subsets, pos_a, pos_b, a, b); // merge two subsets
			mst[*size_mst].source = graph[i].source;
			mst[*size_mst].destination = graph[i].destination;
			mst[*size_mst].weight = graph[i].weight;
			(*size_mst)++;
		}			
		i++;
	}
	if (*size_mst != num_vertex - 1) 
	{
		cout << "It is impossible to create MST\n"; // happens only if the graph was not connected.
		*size_mst = 0;
	}	
	else
	{
		display(mst, *size_mst);
		cost(mst, *size_mst);
	}
}


	
int main()
{
	edge mst[MAX_NUM_EDGES];
	int size_mst = 0;
	create(mst, &size_mst);
	return(0);
}
