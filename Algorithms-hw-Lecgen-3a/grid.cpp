#include "grid.h"

using namespace std;

grid::grid()
{
	matrix<char>::matrix(1, 1, ' ');
	
}



void grid::loadMatrix()
{


	string widthstring;
	string heightstring;
	int width = 0;
	int height = 0;
	string word;
	string params;
	char letter;
	//string letters=NULL;
	vector<vector<char> > mat;


	ifstream gridfile;
	gridfile.open("50x50 matrix.txt");

	gridfile >> height >> width;
	
	
	cout << "the height is " << height << " The width is " << width << endl;



		matrix<char>::matrix(width, height, ' ');
			vector<char> vector;

			while (gridfile.eof() != true)
			{


				getline(gridfile, word);
				stringstream stream(word);
				
				while (stream)
				{
					stream >> letter;
					vector.push_back(letter);
					cout << letter;
				}

				cout << endl;
				mat.push_back(vector);
			}

}


