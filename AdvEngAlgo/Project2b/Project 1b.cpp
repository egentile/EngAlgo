// Project 1b.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include <time.h>
#include "d_except.h"
#include <fstream>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

					  //Graph bestGraph;
int minConflicts;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};
Graph bestGraph;
void initializeGraph(Graph &g, ifstream &fin);

void setNodeWeights(Graph &g, int w);

void exhaustiveColoring(Graph &g, int numColors, int t);

bool color(Graph &g, int numColors, int t, int node, clock_t start);

bool validCheck(Graph &g);

void printSolution(Graph &g);

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	//fileName = "/Users/wmeleis/2560-code/tree2/tree/graph1.txt";

	cout << "Enter filename" << endl;
	cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g;

		int numColors;
		fin >> numColors;
		initializeGraph(g, fin);

		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		exhaustiveColoring(g, numColors, 600);
	}

	catch (int e)
	{

	}

	printSolution(bestGraph);
}

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.  
{
	int n, e;
	int j, k;

	fin >> n >> e;
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int i = 0; i < n; i++)
		v = add_vertex(g);

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
	}

	minConflicts = num_edges(g);
	//set weights to -1
	setNodeWeights(g, -1);
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

void exhaustiveColoring(Graph &g, int numColors, int t)
//sets start time and calls algorithm to exhaustively color graph
{
	clock_t start = clock();
	color(g, numColors, t, 0, start);
}

bool color(Graph &g, int numColors, int t, int node, clock_t start)
//algorithm to exhaustively color graph
{
	if ((clock() - start) / CLOCKS_PER_SEC >= t) {
		return false;
		//print solution?
	}

	for (int i = 0; i < numColors; i++) {
		g[node].weight = i;
		if (validCheck(g)) {
			return true;
		}

		else if ((node + 1) < num_vertices(g))
		{
			if (color(g, numColors, t, node + 1, start))
			{
				return true;
			}
		}
	}
	return false;
}

bool validCheck(Graph &g)
//Checks if graph has valid coloring and updates conflicts
{
	int cons = 0;

	// Get a pair containing iterators pointing the beginning and end of the
	// list of nodes
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	// Loop over all nodes in the graph
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {

		// Get a pair containing iterators pointing to the beginning and end of the
		// list of nodes adjacent to node v
		pair<Graph::adjacency_iterator, Graph::adjacency_iterator>
			vItrRange2 = adjacent_vertices(*vItr, g);

		// Loop over adjacent nodes in the graph
		for (Graph::adjacency_iterator vItr2 = vItrRange2.first; vItr2 != vItrRange2.second; ++vItr2) {
			cout << g[*vItr2].weight << "\n";
			cout << g[*vItr].weight << "\n";
				if (g[*vItr2].weight == g[*vItr].weight) {
					cons++;
					//cout << "\n" << cons << "\n";
				}
			}
		}


	if (cons < minConflicts)
	{
		bestGraph = g;
		minConflicts = cons;
	}

	return minConflicts == 0;
}

void printSolution(Graph &g)
//Prints Best Solution at this point in time
{
	cout << "Conflicts: " << minConflicts << "\n";
	cout << "Node Color Assignments:" << "\n";

	for (int i = 0; i < num_vertices(g); i++) {
		cout << "Node #: " << i << "  Color: " << g[i].weight << "\n";

	}
}



