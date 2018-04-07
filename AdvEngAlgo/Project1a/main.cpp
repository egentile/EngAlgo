// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <stack>
#include <stdlib.h>
#include <time.h>

using namespace std;

#include "d_except.h"
// #include "d_matrix.h"
// #include "graph.h"
#include "knapsack.h"

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;

   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   // fileName = "knapsack16.input";

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

		// clock_t startTime, endTime;
    //
		// int x = 0;
		// startTime = clock();
    //
		// cout << endl << endl << startTime << endl;
		// cout << clock() << endl;
    //
		// int diff = clock() - startTime;
    //
		// cout << (float)diff / CLOCKS_PER_SEC << endl;


      cout << "Reading knapsack instance" << endl;
      knapsack k(fin);

      k.exhaustiveKnapsack(600);

      cout << endl << "Best solution" << endl;
      k.printSolution();

	  system("pause");

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
