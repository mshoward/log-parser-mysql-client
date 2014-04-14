#pragma once

//sets empty strings and isGood to false
hdparser::honeyd_parser::honeyd_parser()
{
	currString = "";
	timeStamp = "";
	packetType = "";
	sourceIP = "";
	sourceSocket = "";
	targetIP = "";
	targetSocket = "";
	osVersion = "";
	isGood = false;
	index = 0;
}

//nothing to free
hdparser::honeyd_parser::~honeyd_parser()
{
	
}

//sets fields
hdparser::honeyd_parser::honeyd_parser(std::string rawString)
{
	setString(rawString);
	errorNumber = 0;
}

//sets fields
void hdparser::honeyd_parser::setString(std::string rawString)
{
	std::stringstream input(rawString, std::ios_base::in);
	currString = rawString;
	std::getline(input, timeStamp, ' ');
	std::getline(input, packetType, ' ');
	std::getline(input, sourceIP, ' ');
	if ((sourceIP == "-") || (sourceIP == "E") || (sourceIP == "S"))
		std::getline(input, sourceIP, ' ');
	std::getline(input, sourceSocket, ' ');
	std::getline(input, targetIP, ' ');
	//std::getline(input, targetSocket, ' ');
	std::getline(input, targetSocket, ':');
	std::getline(input, osVersion); /*
	the end of the log after this gets messy
	and I don't really understand it, so I'll shove it on the
	beginning of osVersion*/
	isGood = true;
	index = 0;
}

void hdparser::honeyd_parser::errPrint()
{
	//std::cout << "The error is: " << errorNumber << std::endl;
}

void hdparser::honeyd_parser::setStringApache(std::string rawString)
{
	isGood = true;
	junkData.clear();
	//std::cout << "dumped junkData... " << std::endl;
	if (pullIP(rawString, sourceIP, ' '))
	{
		//std::cout << "didn't get IP " << std::endl;
		errorNumber = 1;
	}
	else
	{
		//std::cout << "got IP " << std::endl;
	}
	//std::cout << "getting timestamp... " << std::endl;
	int count = 0;
	while ((!(pullTS(rawString, timeStamp, ' '))) && count < 15)
	{
		count++;
		//std::cout << "not TS: " << timeStamp << std::endl;
		junkData = junkData + timeStamp;
	}
	timeStamp = formatTS(timeStamp);
	//std::cout << "TS!!! " << std::endl;
	junkData = junkData + rawString;
	//std::cout << "appended junkdata... " << std::endl;
	//std::cout << "scanning... " << std::endl;
	attackStr = getJunkData();
	//std::cout << "done... " << std::endl;
	isGood = true;
	index = 0;
}
std::string formatTS(std::string ts)
{
	//26/Jun/2013:14:51:05
	//2014-02-02-17:54:28.8605
	
	std::map<std::string, std::string> monthMap;
	monthMap["Jan"] = "01";
	monthMap["Feb"] = "02";
	monthMap["Mar"] = "03";
	monthMap["Apr"] = "04";
	
	monthMap["May"] = "05";
	monthMap["Jun"] = "06";
	monthMap["Jul"] = "07";
	monthMap["Aug"] = "08";
	monthMap["Sep"] = "09";
	monthMap["Oct"] = "10";
	monthMap["Nov"] = "11";
	monthMap["Dec"] = "12";
	std::string ret = "";
	std::string day, month, year, time;
	day = month = year = time = "";
	day = ts.substr(0, ts.find_first_of('/'));
	ts = ts.substr(ts.find_first_of('/') + 1);
	month = monthMap[ts.substr(0, ts.find_first_of('/'))];
	ts = ts.substr(ts.find_first_of('/') + 1);
	year = ts.substr(0, ts.find_first_of(':'));
	ts = ts.substr(ts.find_first_of(':') + 1);
	time = ts.substr(0) + ".0000";
	ret = year;
	ret += "-";
	ret += month;
	ret += "-";
	ret += day;
	ret += "-";
	ret += time;
	//unfinished
	
	return ret;
}

void hdparser::honeyd_parser::omniSetString(std::string &rawString, t_LOGTYPE which)
{
	if (which == HONEY_D)
	{
		//std::cout << "HONEYD OMNISETSTR CALL" << std::endl;
		setString(rawString);
	}
	else
	{
		//std::cout << "APACHE OMNISETSTR CALL" << std::endl;
		setStringApache(rawString);
	}
}

int hdparser::honeyd_parser::getWord(std::string &inStr, std::string &outStr, char delim)
{
	outStr.clear();
	char buf[256];
	int c;
	memset(buf, 0, sizeof(buf));
	std::stringstream input(inStr, std::ios_base::in);
	
	input.getline(buf, 256, delim);
	c = input.gcount();
	//std::cout << buf << std::endl;
	//std::cout << "inserting into str... " << std::endl;
	if (c > 0)
	outStr.insert(0, buf, c - 1);
	//std::cout << "IP END POS: " << input.gcount() << std::endl;
	inStr = inStr.substr(c);
	return c;
}

bool hdparser::honeyd_parser::pullTS(std::string &inStr, std::string &outStr, char delim)
{
	if (getWord(inStr, outStr, delim) > 0)
	{
		outStr = outStr.substr(1);
		return isTS(outStr);
	}
	else
		return false;
}

bool hdparser::honeyd_parser::pullIP(std::string &inStr, std::string &outStr, char delim)
{
	//std::cout << "pulling IP " << std::endl;
	if (getWord(inStr, outStr, delim) > 0)
		return isIP(outStr);
	//std::cout << "didn't get IP " << std::endl;
	return false;
}

hdparser::honeyd_parser::honeyd_parser(std::string rawString, t_LOGTYPE setting)
{
	mode = setting;
	setString(rawString);
}

//returns the timestamp or a string saying 
//the parser's not been given a string
std::string hdparser::honeyd_parser::getTimeStamp()
{
	if(isGood)
		return timeStamp;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}

//same as above
std::string hdparser::honeyd_parser::getPacketType()
{
	if(isGood)
		return packetType;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}

std::string hdparser::honeyd_parser::getSourceIP()
{
	if(isGood)
		return sourceIP;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}
std::string hdparser::honeyd_parser::getTargetIP()
{
	if(isGood)
		return targetIP;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}
std::string hdparser::honeyd_parser::getSourceSocket()
{
	if(isGood)
		return sourceSocket;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}
std::string hdparser::honeyd_parser::getTargetSocket()
{
	if(isGood)
		return targetSocket;
	else
	{
		std::string ret;
		ret = "I HAVE NOT BEEN GIVEN A STRING";
		return ret;
	}
}

std::string hdparser::honeyd_parser::getOsVersion()
{
	if(isGood)
		return osVersion;
	else
	{
		std::string ret;
		ret = "INVALID";
		return ret;
	}
}

bool hdparser::honeyd_parser::isGNumber(char c)
{
	return ((c == '0') || (c == '1') ||
			(c == '2') ||
			(c == '3') ||
			(c == '4') ||
			(c == '5') ||
			(c == '6') ||
			(c == '7') ||
			(c == '8') ||
			(c == '9') );
}

bool hdparser::honeyd_parser::isGAlpha(char c)
{
	return ((c == 'a') || (c == 'A') ||
			(c == 'b') || (c == 'B') ||
			(c == 'c') || (c == 'C') ||
			(c == 'd') || (c == 'D') ||
			(c == 'e') || (c == 'E') ||
			(c == 'f') || (c == 'F') ||
			(c == 'g') || (c == 'G') ||
			(c == 'h') || (c == 'H') ||
			(c == 'i') || (c == 'I') ||
			(c == 'j') || (c == 'J') ||
			(c == 'k') || (c == 'K') ||
			(c == 'l') || (c == 'L') ||
			(c == 'm') || (c == 'M') ||
			(c == 'n') || (c == 'N') ||
			(c == 'o') || (c == 'O') ||
			(c == 'p') || (c == 'P') ||
			(c == 'q') || (c == 'Q') ||
			(c == 'r') || (c == 'R') ||
			(c == 's') || (c == 'S') ||
			(c == 't') || (c == 'T') ||
			(c == 'u') || (c == 'U') ||
			(c == 'v') || (c == 'V') ||
			(c == 'x') || (c == 'X') ||
			(c == 'y') || (c == 'Y') ||
			(c == 'z') || (c == 'Z') );
}

bool hdparser::honeyd_parser::isSymbol(char c, char symbol)
{
	return (c == symbol);
}

bool hdparser::honeyd_parser::isTimeStamp(std::string s)
{
	//2014-02-02-17:54:29.3535
	//0123456789 11131517192123
	//          10121416182022
	int ptr = 0;
	bool ret = true;
	while (ptr < s.length() && ret)
	{
		if (ptr < 4)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 5)
		{
			ret = ret && isSymbol(s[ptr], '-');
		}
		else if (ptr < 7)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 8)
		{
			ret = ret && isSymbol(s[ptr], '-');
		}
		else if (ptr < 10)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 11)
		{
			ret = ret && isSymbol(s[ptr], '-');
		}
		//2014-02-02-17:54:29.3535
		//0123456789 11131517192123
		//          10121416182022
		else if (ptr < 13)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 14)
		{
			ret = ret && isSymbol(s[ptr], ':');
		}
		else if (ptr < 16)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 17)
		{
			ret = ret && isSymbol(s[ptr], ':');
		}
		else if (ptr < 19)
		{
			ret = ret && isGNumber(s[ptr]);
		}
		else if (ptr < 20)
		{
			ret = ret && isSymbol(s[ptr], '.');
		}
		//2014-02-02-17:54:29.3535
		//0123456789 11131517192123
		//          10121416182022
		else if (ptr < s.length())
		{
			ret = ret && (isGNumber(s[ptr]) || isSymbol(s[ptr], (char) 32) || isSymbol(s[ptr], (char) 9) || isSymbol(s[ptr], (char) 0));
		}
		ptr++;
	}
	return ret;
}
bool hdparser::honeyd_parser::isPacketType(std::string s)
{
	bool ret = true;
	for(int i = 0; (i < s.length()) && ret; i++)
	{
		if (isGAlpha(s[i]) || isSymbol(s[i], (char) 0))
		{
			ret = ret && true;
		}
		else
		{
			if (isSymbol(s[i], '('))
			{
				i++;
				bool cont = true;
				for(;((i < s.length()) && ret) && cont; i++)
				{
					//std::cout << "REJECTING BECAUSE " << s[i] << " isn't a (" << std::endl;
					ret = ret && (isGNumber(s[i]) || isSymbol(s[i], ')'));
					if (isSymbol(s[i], ')'))
						cont = false;
				}
			}
			else
			{
				//std::cout << "REJECTING BECAUSE " << s[i] << "isn't a (" << std::endl;
				ret = ret && false;
			}
		}
	}
	//std::cout << "END OF CALL " << std::endl;
	return ret;
}
bool hdparser::honeyd_parser::isIP(std::string s)
{
	bool ret = true;
	int temp = 0;
	for(int i = 0; (i < s.length()) && ret; i++)
	{
		//std::cout << "READING:" << s[i] << std::endl;
		//std::cout << "TEMP = " << temp << std::endl;
		
		
		if (isGNumber(s[i]))
		{
			//std::cout << s[i] << " is a number" << std::endl;
			temp++;
		}
		else
		{
			if (temp == 3)
			{
				ret = ret && (isSymbol(s[i], '.') || isSymbol(s[i], ' ') || isSymbol(s[i], (char) 0));
				//if (!(isSymbol(s[i], '.') || isSymbol(s[i], ' ') || isSymbol(s[i], (char) 0)))
					//std::cout << "REJECTING IP BECUASE :" << s[i] << std::endl;
			}
			else if (isSymbol(s[i], '.'))
			{
				temp = 0;
			}
			
			else
			{
				ret = false;
				//std::cout << "REJECTING IP BECUASE :" << s[i] << std::endl;
			}
		}
		
		
	}
	return ret;
}
bool hdparser::honeyd_parser::isSocket(std::string s)
{
	bool ret = true;
	for(int i = 0; i < s.length(); i++)
	{
		ret = ret && (isGNumber(s[i]) || isSymbol(s[i], ':') || isSymbol(s[i], (char) 0) || isSymbol(s[i], ' '));
	}
	return ret;
}

bool hdparser::honeyd_parser::isOSVersion(std::string s)
{
	return true;
}


bool hdparser::honeyd_parser::getValidTimeStamp()
{
	//2014-02-02-17:54:29.3535
	return isTimeStamp(timeStamp);
}

bool hdparser::honeyd_parser::getValidPacketType()
{
	return isPacketType(packetType);
}

bool hdparser::honeyd_parser::getValidSourceIP()
{
	return isIP(sourceIP);
}

bool hdparser::honeyd_parser::getValidSourceSocket()
{
	return isSocket(sourceSocket);
}

bool hdparser::honeyd_parser::getValidTargetIP()
{
	return isIP(targetIP);
}

bool hdparser::honeyd_parser::getValidTargetSocket()
{
	return isSocket(targetSocket);
}

bool hdparser::honeyd_parser::getValidOsVersion()
{
	return isOSVersion(osVersion);
}

std::string hdparser::honeyd_parser::getJunkData()
{
	std::string ret = "";
	hdparser::honeyd_parser::t_ATTACK_TYPE type = getJunkDataAttackType();
	if (type == SQL_INJECTION)
	{
		ret = "SQL-INJECTION";
		return ret;
	}
	else if (type == XSS)
	{
		ret = "CROSS-SITE SCRIPTING";
		return ret;
	}
	else if (type == DUBIOUS_PHP)
	{
		ret = "DUBIOUS PHP - POSSIBLE VULN.";
		return ret;
	}
	else
	{
		ret = "HTTP EXPOIT";
		return ret;
	}
	return ret;
}
hdparser::honeyd_parser::t_ATTACK_TYPE hdparser::honeyd_parser::getJunkDataAttackType()
{
	int sqlInjects = -1;
	std::string scripts = "SCRIPT";
	std::string temp = toUpper(junkData.substr(0, -1));
	
	for (int i = 0; (i < parser_sqli::SQLI_LIST_LENGTH) && (sqlInjects <= 5); i++)
	{
		//std::cout << toUpper(parser_sqli::SQLI_LIST[i]) << std::endl;
		//std::cout << temp << std::endl;
		sqlInjects = (temp.find(toUpper(parser_sqli::SQLI_LIST[i]), 0));
		/*if (sqlInjects != -1)
			std::cout << parser_sqli::SQLI_LIST[i] << std::endl;*/
	}
	if (!(sqlInjects <= 0))
		return SQL_INJECTION;
	sqlInjects = temp.find(scripts, 0);
	if (!(sqlInjects <= 0))
		return XSS;
	scripts = "PHP";
	sqlInjects = temp.find(scripts, 0);
	if (!(sqlInjects <= 0))
		return DUBIOUS_PHP;
	else
		return HTTP_ETC;
}

std::string hdparser::honeyd_parser::toUpper(std::string strToBeUpper)
{
	std::string ret = "";
	for(int i = 0; i < strToBeUpper.length(); i++)
	{
		ret = ret + (char) std::toupper(strToBeUpper[i]);
	}
	return ret;
}
hdparser::honeyd_parser::WORDTYPE hdparser::honeyd_parser::getWordType(std::string s)
{
	if (isTimeStamp(s))
		return TIMESTAMP;
	if (isPacketType(s))
		return PACKETTYPE;
	if (isIP(s))
		return IP;
	if (isSocket(s))
		return SOCKET;
	if (isOSVersion(s))
		return OSVERSION;
	return JUNKDATA;
}

bool hdparser::honeyd_parser::isTS(std::string s)
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


