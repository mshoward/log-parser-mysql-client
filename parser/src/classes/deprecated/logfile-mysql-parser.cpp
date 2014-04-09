#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//honeypot1.db.7456864.hostedresource.com
/*
	DEPRECATED
*/
int main()
{
	fstream f1("g++help.txt", ios::in);
	stringstream ss = 
	string str;
	while (!f1.eof())
	{
		getline(f1, str);
		
		cout << str << endl;
	}
	
	return 0;
}
