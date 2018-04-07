#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include "d_matrix.h"

using namespace std;

class grid
{
private:
	//vector<char> mat(vector<char> ;
	//???????

	int width, height;
	string word;

public:

	// default constructor
	grid();

	// default destructor
	//~grid();

	// read letters in the grid -- stores them in a matrix
	void loadMatrix();

};