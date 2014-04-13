#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
using namespace std;

bool isIP(string s);
bool isTS(string s);

int getWord(std::string &inStr, std::string &outStr, char delim)
{
	outStr.clear();
	char buf[256];
	int c;
	memset(buf, 0, sizeof(buf));
	std::stringstream input(inStr, std::ios_base::in);
	input.getline(buf, 256, delim);
	c = input.gcount();
	outStr.insert(0, buf, c - 1);
	//cout << "IP END POS: " << input.gcount() << endl;
	inStr = inStr.substr(c);
	return c;
}


bool PullIP(string &inStr, string &outStr)
{
	outStr.clear();
	char buf[256];
	memset(buf, 0, sizeof(buf));
	std::stringstream input(inStr, std::ios_base::in);
	input.getline(buf, 256, ' ');
	outStr.insert(0, buf, input.gcount() - 1);
	cout << "IP END POS: " << input.gcount() << endl;
	inStr = inStr.substr(input.gcount());
	return isIP(outStr);
}

bool isIP(string s)
{
	bool ret = true;
	int temp = 0;
	for(int i = 0; (i < s.length()) && ret; i++)
	{
		if (isdigit(s[i]))
		{
			//cout << "isDigit " << s[i] << endl;
			temp++;
		}
		else
		{
			//cout << "is not digit: " << s[i] << endl;
			if (temp == 3)
			{
				ret = ret && ((s[i] == '.') || (s[i] == ' ') || (s[i] == (char) 0));
				//if (!ret) cout << "is broken: " << s[i];
			}
			else if (s[i] == '.')
			{
				temp = 0;
			}
			
			else
			{
				ret = false;
				//cout << "is broken: "  << s[i];
			}
		}
		
		
	}
	return ret;
}

bool PullTS(std::string &inStr, std::string &outStr)
{
	if (getWord(inStr, outStr, ' ') > 0)
	{
		outStr = outStr.substr(1);
		return isTS(outStr);
	}
	else
		return false;
}

std::string formatTS(std::string ts)
{
	std::string ret = "test";
	return ret;
}

int main()
{
	string num = "12345";
	cout << (atoi(num.c_str())) + 1 << endl;
	string ln = "192.168.188.138 - - [26/Jun/2013:14:51:05 -0700] \"GET /joomla/ HTTP/1.1\" 200 20303 \"http://192.168.188.128/\" \"Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:14.0) Gecko/20100101 Firefox/14.0.1\"";
	string myStr = "192.168.1.1 is awesome!";
	string myOther = "";
	cout << "line before anything: " << endl;
	cout << ln << endl;
	cout << endl << endl;
	cout << "IP: " << endl;
	while(!(PullIP(ln, myOther))) ;
	cout << myOther << endl;
	cout << "TS: " << endl;
	while(!(PullTS(ln, myOther))) ;
	cout << myOther << endl;
	cout<< ln << endl;
	
	
	
	return 0;
}

bool isTS(string s)
{
	//2014-02-02-17:54:29.3535
	//0123456789 11131517192123
	//          10121416182022
	//[26/Jun/2013:14:51:05
	//cout << s << endl;
	int digits = 0;
	int chars = 0;
	bool val = true;
	int STATE = 0;
	// 1:day, 2:month, 3:year, 4:hour, 5:min, 6:sec
	for(int i = 0; (i < s.length()) && val; i++)
	{
		//cout << STATE << endl;
		if ((STATE == 0))
		{
			if(isdigit(s[i]))
			{
				val = true;
			}
			else if (s[i] == '/')
			{
				val = true;
				STATE = 1;
			}
			else
			{
				val = false;
			}
		}
		else if ((STATE == 1))
		{
			if(isalpha(s[i]))
			{
				val = true;
			}
			else if (s[i] == '/')
			{
				val = true;
				STATE = 2;
			}
			else
			{
				val = false;
			}
		}
		else if ((STATE == 2))
		{
			if(isdigit(s[i]))
			{
				val = true;
			}
			else if (s[i] == ':')
			{
				val = true;
				STATE = 3;
			}
			else
			{
				val = false;
			}
		}
		else if ((STATE == 3))
		{
			if(isdigit(s[i]))
			{
				val = true;
			}
			else if (s[i] == ':')
			{
				val = true;
				STATE = 4;
			}
			else
			{
				val = false;
			}
		}
		else if ((STATE == 4))
		{
			if(isdigit(s[i]))
			{
				val = true;
			}
			else if (s[i] == ':')
			{
				val = true;
				STATE = 5;
			}
			else
			{
				val = false;
			}
		}
		else if ((STATE == 5))
		{
			if(isdigit(s[i]))
			{
				val = true;
				STATE = 6;
			}
		}
	}
	//cout << STATE << endl;
	return STATE == 6;
}
