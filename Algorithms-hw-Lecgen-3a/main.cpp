// LecGen-3a.cpp : Defines the entry point for the console application.
//


#include "dictionary.h"
#include "grid.h"

using namespace std;

int main()
{
	dictionary dictionary1;
	dictionary1.loadDictionary();
	cout << dictionary1;
	

	//grid grid1;

//	gid1.loadMatrix();
	
	system("pause");
    return 0;
}

