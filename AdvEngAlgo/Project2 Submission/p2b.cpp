// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <math.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

int conflicts;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
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

bool checkValidity(Graph &g);

// bool colorGraph(Graph&g, int numColors, int timeLength, int nodeStart, clock_t startinTime);

void checkColoredGraph(Graph &g)
{
  conflicts = 0;

  for(int i = 0; i < num_vertices(g); i++)
  {

      for( int j = 0; j < num_vertices(g); j++)
      {

          if(i == j)
          {

          }

          else if((edge(i, j, g)).second)
          {
              if(g[i].weight == g[j].weight)
              {
                conflicts += 1;
              }

          }

      }

  }
}

void greedyNodeColor(Graph &g, int nodeNum, int numberOfColors)
{
  int weightNeighbor;
  int multiplier;

  for (int i = 0; i < num_vertices(g); i++)
  {


    if(nodeNum == i)
    {

    }

    else if ((edge(nodeNum, i , g)).second)//|| edge(i, nodeNum, g))
    {
      if((edge(i, nodeNum, g).second))
      {

        weightNeighbor |= (1 << g[i].weight);
        // multiplier = g[i].weight;
        //
        // if(multiplier == 1)
        // {
        //   weightNeighbor = 1;
        // }
        //
        // else
        // {
        //   weightNeighbor = pow(2,multiplier);
        // }
      }
    }

  }

  for (int j = 0; j < num_vertices(g); j++)
  {
    if(weightNeighbor & 1 << j == 0)
    {
      g[nodeNum].weight = j;
      return;
    }
  }
  g[nodeNum].weight = 0;
}

void greedyColor(Graph &g, int numColors)
{
  for(int x = 0; x < num_vertices(g); x++)
  {
    g[x].weight = numColors + 1;
  }

  for (int i = 0; i < num_vertices(g); i++)
  {
    greedyNodeColor(g, i, numColors);
  }

  checkColoredGraph(g);

}

void printSolution(Graph &g);

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].weight = w;
    }
}

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
    int n, e;
    int j,k;

    fin >> n >> e;
    Graph::vertex_descriptor v;

    // Add nodes.
    for (int i = 0; i < n; i++)
        v = add_vertex(g);

    for (int i = 0; i < e; i++)
    {
        fin >> j >> k;
        add_edge(j,k,g);  // Assumes vertex list is type vecS
    }

    conflicts = num_edges(g);

    setNodeWeights(g,-1);
}

// bool colorGraph(Graph &g, int numColors, int timeLength, int nodeStart, clock_t startinTime)
// {
//
// // check if 10 mins have passed
// if( (clock() - startinTime)/ CLOCKS_PER_SEC >= timeLength)
// {
//   return false;
// }
//
// for (int j = 0; j< numColors; j++)
// {
//   g[nodeStart].weight = j;
//
//   if (checkValidity(g))
//   {
//     return true;
//   }
//
//   else if ((nodeStart+1) < num_vertices(g))
//   {
//
//       if (colorGraph(g,numColors, timeLength, nodeStart + 1, startinTime))
//       {
//         return true;
//       }
//
//   }
//
//   else
//   {
//     return false;
//   }
//
// }

// }

// bool checkValidity(Graph &g)
// {
//     int conflicts = 0;
//
// 	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
//
// 	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
//
// 		pair<Graph::adjacency_iterator, Graph::adjacency_iterator>
// 			vItrRange2 = adjacent_vertices(*vItr, g);
//
// 		for (Graph::adjacency_iterator vItr2 = vItrRange2.first; vItr2 != vItrRange2.second; ++vItr2) {
// 			cout << g[*vItr2].weight << "\n";
// 			cout << g[*vItr].weight << "\n";
// 				if (g[*vItr2].weight == g[*vItr].weight) {
// 					conflicts++;
// 				}
// 			}
// 		}
//
//
// 	if (conflicts < minimumConf)
// 	{
// 		bestGraph = g;
// 		minimumConf = conflicts;
// 	}
//
// 	return 0 == minimumConf;
// }


// void exhaustiveColoring(Graph &g, int numColors, int timeLength)
// {
//   clock_t startingTime = clock();
//   colorGraph(g,numColors,timeLength,0,startingTime);
// }

void printSolution(Graph &g)
//Prints Best Solution at this point in time
{
	cout << "Conflicts: " << conflicts << "\n";
	cout << "Node Color Assignments:" << "\n";

	for (int i = 0; i < num_vertices(g); i++) {
		cout << "Node #: " << i << "  Color: " << g[i].weight << "\n";

	}
}


int main()
{
    char x;
    ifstream fin;
    string fileName;

    // Read the name of the graph from the keyboard or
    // hard code it here for testing.

    fileName = "graph12-3.txt";

    //   cout << "Enter filename" << endl;
    //   cin >> fileName;

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
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(g,fin);

        cout << "Num nodes: " << num_vertices(g) << endl;
        cout << "Num edges: " << num_edges(g) << endl;
        cout << endl;

        // cout << g;

        // exhaustiveColoring(g, numColors, 600);
        greedyColor(g, numColors);
        printSolution(g);

    }
    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}
