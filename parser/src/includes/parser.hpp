#pragma once

#define PATH "/home/mason/gits/lpmc/log-parser-mysql-client/parser/src/conf/lpmc.conf"
typedef struct
{
	std::string honeyd_log_filePath;
	std::string apache_log_filePath;
	std::string cache_filePath;
	//std::string * otherFiles; 
} s_configuration;

s_configuration configuration;

bool firstCharIsHash(std::string s)
{
	for(int i = 0; i < s.length(); i++)
	{
		if (!isspace(s[i]))
		{
			return (s[i] == '#');
		}
	}
	return true;
}

std::string nextUsefulLine(std::fstream &str, std::string &s)
{
	//int i = 0;
	std::getline(str, s);
	//std::cout << "nextUseful extract[" << i << "]: " << s << std::endl;
	while ((firstCharIsHash(s)) && !(str.eof()))
	{
		std::getline(str, s);
		//i++;
		//std::cout << "nextUseful extract[" << i << "]: " << s << std::endl;
	}
	return s;
}



void getConfiguration()
{
	std::fstream conf(PATH, std::ios_base::in);
	std::string ln = "";
	
	if (!(conf.is_open()))
	{
		fprintf(stderr, "Config file not found.  Exiting lpmc.");
		printf("Config file not found.  Exiting lpmc.\n");
		/** exit 2 means it can't find the config file */
		exit(2);
	}
	else
	{
		nextUsefulLine(conf, ln);
		if (ln != "BEGINCONF")
		{
			fprintf(stderr, "Config file not formatted correctly.");
			printf("Config file not formatted correctly.");
			/** exit 3 means config file's not formatted correctly. */
			exit(3);
		}
		nextUsefulLine(conf, ln);
		configuration.honeyd_log_filePath = ln.substr(0);
		
		nextUsefulLine(conf, ln);
		configuration.apache_log_filePath = ln.substr(0);
		
		nextUsefulLine(conf, ln);
		configuration.cache_filePath = ln.substr(0);
		//if (ln == "ENDCONF") std::cout << "ENDCONF\n";
	}
	conf.close();
	
}

void dumpFile(std::string &fpath)
{
	std::remove(fpath.c_str());
	system( ("touch " + fpath).c_str());
	chmod(fpath.c_str(), 
		S_IRUSR | S_IWUSR |
		S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH );
}


void flushCache(std::fstream &cache, mysqlPush &sql)
{
	if (sql.isConnected())
	{
		std::string bucket = "";
		while ((!(cache.eof())) && (sql.isConnected()))
		{
			if (cache.peek() != ';')
			{
				bucket += cache.get();
			}
			else
			{
				bucket += cache.get();
				sql.execQuery(bucket);
				bucket.clear();
			}
			//std::getline(cache, bucket);
			//sql.execQuery(bucket);
		}
		if (!(sql.isConnected()))
			exit(666);
	}
	else
	{
		exit(666);
	}
}






















