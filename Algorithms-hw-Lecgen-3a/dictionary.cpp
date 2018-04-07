#include "dictionary.h"

using namespace std;

ostream& operator<<(ostream &os, const dictionary &dict) //, vector<string> wordList)
{
	for (int i = 0; i < sizeof(dict.wordList); i++)
	{
		os << dict.wordList[i] << endl;
	}

	return os;
}

void dictionary::loadDictionary()
{
	//ifstream infile("Dictionary.txt");
	ifstream infile("testdictionary.txt");
	if (infile)
	{
		string word;

		while( infile.eof() != true)
		{
			getline(infile, word);
			wordList.push_back(word);
		}

	}

}

void dictionary::sortDictVector(vector<string> dictVector)
{
	int size = dictVector.size();
	int word1 = 0;
	int min = 0;
	int word2 = 1;
	int count = 0;
	string temp;

	while (count != size) // ??
	{
		for (int i = 0; i < size; i++)
		{
			if (word1biggerthanword2(dictVector[word1], dictVector[word2]))
			{
				min = word2;
			}
			word1++;
			word2++;
		}

		temp = dictVector[count];
		dictVector[count] = dictVector[min];
		dictVector[min] = temp;

		count++;
		word1 = count;
		word2 = word1 + 1;
	}

}

bool dictionary::word1biggerthanword2(string string1, string string2)
{
	for (int i = 0; i < string1.length() || i < string2.length(); i++)
	{
		if (string1[i] < string2[i])
		{
			return true;
		}

		else if (string1[i] > string2[i])
		{
			return false;
		}

	}

	if (string1.length() < string2.length())
	{
		return true;
	}

	else
	{
		return false;
	}
}
