/* Erik Gentile and Malcolm Leclair
* Project 5b
* April 5, 2017
* Engineering Algorithms
*/

/********************main.cpp**********************
* Contains main file and maze Class
***************************************************/

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class maze
{
   public:
      maze(ifstream &fin);
      void print(int,int,int,int);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);
      int getMap(int i, int j) const;
      void mapMazeToGraph(graph &g);
      void mapLookup(int &x, int &y, int n);
      void renderSolution(vector<int> p, int gx, int gy);
      vector<int> solveIterative(graph &g, int finX, int finY);
      void printDirections(vector<int> p);

      vector<int> solveRecursive(graph &g);
      void dfs(graph &g, int id);
      bool findShortestPath1(graph &g); // bfs
      bool findShortestPath2(graph &g); // dijkstra's
      int getRows(){return rows;};
      int getCols(){return cols;};

      struct Comparitor
      {
      public:
        bool operator()(node &n1, node &n2)
        {
          return n1.getWeight() > n2.getWeight(); // sign?? < or >
        }
      };

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze
      bool solnFound;

      matrix<bool> value;
      matrix<int> map; // Mapping from maze (i,j) values to node index values
      vector<int> recursiveVector;
      vector<int> bfsVector;
  };

void maze::mapLookup(int &x, int &y, int n)
// set x and y to the coordinates of a node n
{
    for (int i = 0; i <= rows-1; i++)
        for (int j = 0; j <= cols-1; j++)
            if(getMap(i,j) == n)
            {
                x = i;
                y = j;
            }
}// end mapLookup

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
    map[i][j] = n;
}// end setMap

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{

    if (i < 0 || i > rows - 1 || j < 0 || j > cols - 1)
    {
       throw rangeError("Bad value in maze::getMap");
    }
    return map[i][j];
}// end getMap

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;

    char x;

    value.resize(rows,cols);
    map.resize(rows,cols);
    for (int i = 0; i <= rows-1; i++)
        for (int j = 0; j <= cols-1; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;

            // initialize all map values to -1
            map[i][j] = -1;
        }

    map.resize(rows,cols);
}// end maze

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
    cout << endl;

    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");

    if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
        throw rangeError("Bad value in maze::print");

    for (int i = 0; i <= rows-1; i++)
    {
        for (int j = 0; j <= cols-1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
                if (i == currI && j == currJ)
                    cout << "+";
                else
                    if (value[i][j])
                        cout << " ";
                    else
                        cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}// end print

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
   if (i < 0 || i > rows || j < 0 || j > cols)

      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}// end isLegal

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    // build nodes
    for(int i = 0; i <= rows-1; i++)
        for(int j = 0; j <= cols-1; j++)
        {
            if(isLegal(i, j))
            {
                // mapMazeToGraph(i,j,n) adds the node to map[i][j]
                // g.addNode(weight) adds the node to the graph at next index
                setMap(i, j, g.addNode(1));
            }
        }
    // build edges
    int map;
    int node;

    for(int i = 0; i <= rows-1; i++)
        for(int j = 0; j <= cols-1; j++)
        {
            node = getMap(i, j);
            // 4 cases of adjacency
            // i++, j
            // i--, j
            // i, j++
            // i, j--
            if(node != -1)
            {
                if(i + 1 <= rows-1)
                {
                    map = getMap(i + 1, j    );
                    if(map != -1)
                        g.addEdge(node, map, 1);
                }

                if(i - 1 >= 0)
                {
                    map = getMap(i - 1, j    );
                    if(map != -1)
                        g.addEdge(node, map, 1);
                }

                if(j + 1 <= cols-1)
                {
                    map = getMap(i    , j + 1);
                    if(map != -1)
                        g.addEdge(node, map, 1);
                }

                if(j - 1 >= 0)
                {
                    map = getMap(i    , j - 1);
                    if(map != -1)
                        g.addEdge(node, map, 1);
                }
            }
        }
}// end mapMazeToGraph

vector<int> maze::solveRecursive(graph &g)
// returns a path of nodes (vector of node indices) generated using recursive
// dfs
{
    solnFound = false;

    for (int i = 0; i <= rows; i++)
    {
        g.unVisit(i);
    } // makes sure all nodes are unvisited to start
    dfs(g, 0);
    return recursiveVector;
}// end solveRecursive

ostream& operator<<(ostream &os, vector<int> p)
{
    for(int i = 0; i < p.size()-1; i++)
        os << p.at(i) << ", ";
    os << p.back() << endl;
    return os;
}// end operator<<

void maze::dfs(graph &g, int id)
// returns a path of nodes (vector of node indices) generated using iterative
// dfs
{
    if(solnFound == false)
    {
        g.visit(id);
        recursiveVector.push_back(id);

        if(id == getMap(rows-1,cols-1))
        {
            solnFound = true;
        }

        for(int i = 0; i < g.numNodes(); i++)
        {
            if(g.isEdge(id, i) && !(g.isVisited(i)))
            {
                dfs(g, i);
            }
        }
    if(solnFound == false)
        recursiveVector.pop_back();
    }

} // end dfs


bool maze::findShortestPath1(graph &g)
// finds the shortest path to all nodes using bfs
// and a parent vector to create the path
{
  vector<int> parentVector;
  vector<int> reversedPath;
  vector<int> path;

  // unvisits all nodes
  for (int i = 0; i < g.numNodes(); i++)
  {
      g.unVisit(i);
  }

  g.visit(getMap(0,0)); // marks starting node as visited

  // initializes the vector q which contains the frontier
  queue<int> q;
  q.push(getMap(0,0));
  parentVector.push_back(getMap(0,0));

  // fills the parentVector with impossible parent nodes
  for(int i = 0; i < rows*cols - 1; i++)
    parentVector.push_back(-1);

  int v, newParent;

  // searches the frontier until its empty
  while(q.empty() == false)
  {
    //get first element in frontier
    v = q.front();
    // break if found
    if(v == getMap(rows-1,cols-1))
      break;

    for(int i = 0; i < g.numNodes(); i++)
      {
          if(g.isEdge(v, i) && !(g.isVisited(i)))
          {
            g.visit(i);
            q.push(i);
            parentVector[i] = v;
          }
      }

    newParent = v;
    q.pop();
  }

  int id = v;
  parentVector[0] = -1;

  // extracts the reversed path
  while(1!=0)
  {
    if(parentVector[id] == -1)
    {
      reversedPath.push_back(0);
      break;
    }
    else
    {
      reversedPath.push_back(id);
      id = parentVector[id];
    }
  }

  // creates the path
  for(int k = reversedPath.size() - 1; k >= 0; k--)
  {
    path.push_back(reversedPath[k]);
  }

  // if a solution exists display it and return true
  if(path.back() != 0)
  {
    renderSolution(path, getRows() - 1, getCols() - 1);
    printDirections(path);
    return true;
  }

  return false;
} // end findShortestPath1 (bfs)

bool maze::findShortestPath2(graph &g)
// finds the shortest path to all nodes using dijkstra's algorithm
// and a parent vector to create the path
{
  vector<int> path;
  vector<int> parentVector(g.numNodes(), -1);
  cout << "THERE ARE " << g.numNodes() << " NODES" << endl;
  vector<int> reversedPath;
  int v;

  // unvisits all nodes
  for (int i = 0; i < g.numNodes(); i++)
  {
      g.unVisit(i);
      g.setNodeWeight(i, 100000);
  }

  // visit the node and set its weight to 0?
  g.visit(getMap(0,0));
  g.setNodeWeight(0,0);

  // put all nodes into priority_queue
  priority_queue<node, vector<node>, Comparitor> pq;
  for(int j = g.numNodes()-1; j >= 0; j--) // pushes node objects into pq
  {
    pq.push(g.getNode(j));
  }

  // search the list and make a parentList
  while(pq.top().getId()!=g.numNodes()-1)
  {
    v = pq.top().getId();
    g.visit(v);
    pq.pop();
    // update the other nodes with the distance to this current node
    for(int i = 0; i < g.numNodes(); i++)
    {
      if((g.isEdge(v,i) == true) && (g.getNodeWeight(i) > g.getNodeWeight(v) + g.getEdgeWeight(v,i)))
      {
          // cout << "THERE IS AN EDGE FROM " << v << " TO " << i << endl;
          g.setNodeWeight(i, g.getNodeWeight(v) + g.getEdgeWeight(v,i));
          pq.push(g.getNode(i));
          parentVector[i] = v;
      }
    }
  }

  int id = g.numNodes() -1;
  parentVector[0] = -1;
  int count = 0;

  // extracts the reversed path
  while(1!=0)
  {
    if(parentVector[id] == -1)
    {
      reversedPath.push_back(0);
      break;
    }

    else
    {
      reversedPath.push_back(id);
      id = parentVector[id];
    }

    count++;

    if(count == 100)
      break;
  }

  // creates the path
  for(int k = reversedPath.size() - 1; k >= 0; k--)
  {
    path.push_back(reversedPath[k]);
  }

  // if path found renders the solution and returns true
  if(path.back() != 0)
  {
    renderSolution(path, getRows() - 1, getCols() - 1);
    printDirections(path);
    return true;
  }

  return false;
}// end findShortestPath1 (dijkstra)

vector<int> maze::solveIterative(graph &g, int finX, int finY)
// returns a path of nodes (vector of node indices) generated using iterative
// stacke based dfs
{
    int start = getMap(0, 0);
    int fin = getMap(finX, finY);
    vector<int> path;

    // mark all vertices as unvisited
    for(int i = 0; i < g.numNodes(); i++)
        g.unVisit(i);
    // push_back the start vertex on the stack
    path.push_back(start);
    // mark the start vertex as visited
    g.visit(start);

    int v;
    while(path.size() != 0)
    {
        v = path.back();
        if(v == fin)
            return path;

        // for all neighbors
        for(int i = 0; i < g.numNodes(); i++)
        {
            if(g.isEdge(v,i) && !(g.isVisited(i)))
            {
                // mark node visited
                // push node to stack
                g.visit(i);
                path.push_back(i);
                break;
            }
        }
        if(path.back() == v)
        {
            path.pop_back();
        }
    } // end while
    return path;
} // end solveIterative

void maze::renderSolution(vector<int> p, int gx, int gy)
// prints the maze and the current position at each step
{
    int l = p.size();
    cout << "The Path is " << l << " steps."<< endl;
    int x = 0, y = 0;
    for(int i = 0; i < l; i ++)
    {
        mapLookup(x, y, p.at(i));
        print(gx, gy, x, y);
    }
} //end renderSolution

void maze::printDirections(vector<int> p)
// prints the direction of each step
{
    int x, y;
    int px, py = p.at(0);
    cout << "The directions are: " << endl;
    for(int i = 1; i < p.size(); i++)
    {
        mapLookup(x,y,p.at(i));
        if(x == px++)
            cout << "right" << endl;
        if(x == px--)
            cout << "left" << endl;
        if(y == py--)
            cout << "up" << endl;
        if(y == py--)
            cout << "down" << endl;
        px = x;
        py = y;
    }
}// end printDirections

void solveMaze(string); // forward declaration for solveMaze function

int main()
{
    string fileName1 = "Maze1.txt";
    string fileName2 = "Maze2.txt";
    string fileName3 = "Maze3.txt";

    solveMaze(fileName1);
    solveMaze(fileName2);
    solveMaze(fileName3);

} //end main

void solveMaze(string fileName)
{
    char x;
    ifstream fin;

    // Read the maze from the file.

    fin.open(fileName.c_str());
    if (!fin)
    {
       cerr << "Cannot open " << fileName << endl;
       exit(1);
    }

    try
    {

       graph g;
       while (fin && fin.peek() != 'Z')
       {
          maze m(fin);
          cout << "The maze to solve is" << endl;
          m.print(0,0,0,0);
          graph g1;
          // make graph
          m.mapMazeToGraph(g1);

          //solve using bfs then dijkstra
          m.findShortestPath1(g1);
          m.findShortestPath2(g1);

       } //end while

    } //end try
    catch (indexRangeError &ex)
    {
       cout << ex.what() << endl; exit(1);
    } // end catch
    catch (rangeError &ex)
    {
       cout << ex.what() << endl; exit(1);
    } //end catch
} // end solveMaze
