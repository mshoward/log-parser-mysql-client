#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdio.h>
#include <cstring>
using namespace std;
/** both strings should be null terminated */
int matchCount(string strToMatch, string filePath)
{
	//int res = 0;
	string com = "grep -c \"";
	//tcp\" ./../samples/logfile.txt"
	com = com + strToMatch;
	com = com + '"';
	com = com + ' ' + filePath;
	FILE *myfile;
	myfile = popen(com.c_str(), "r");
	string output = "";
	int c = !EOF;
	while (c != EOF)
	{
		c = fgetc(myfile);
		if (c != EOF)
		{
			output.append((char *)&c, 1);
		}
	}
	stringstream s(output, ios_base::in);
	s >> c;
	return c;
}

string toUpper(string strToBeUpper)
{
	string ret = "";
	for(int i = 0; i < strToBeUpper.length(); i++)
	{
		ret = ret + (char) std::toupper(strToBeUpper[i]);
		
	}
	return ret;
}

int main()
{
	
	string searchTerm = "";
	//cin >> searchTerm;
	getline(cin, searchTerm);
	searchTerm = toUpper(searchTerm);
	cout << searchTerm << endl;
	//int c = matchCount(searchTerm, "./../samples/logfile.txt");
	//cout << c << endl;
	return 0;
}



