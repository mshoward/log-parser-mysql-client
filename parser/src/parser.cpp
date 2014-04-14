#include "parser-includes.hpp"

using namespace parser_sqli;
using namespace hdparser;

void main_init()
{
	if (mysql_library_init(0, NULL, NULL)) 
	{
		fprintf(stderr, "could not initialize MySQL library\n");
		std::cout << "could not initialize MySQL library" << std::endl;
		/** exit status 1 means the mysql library couldn't initialize */
		exit(1);
	}
	getConfiguration();
}

void main_exit()
{
	//std::cout << "didn't segfault yet " << std::endl;
	mysql_library_end();
}


void safe_NextPopFields(honeyd_parser &someStmt, honeyd_parser::t_LOGTYPE t,
						std::string &someString, std::fstream &someStream)
{
	//std::cout << "getting line pt 1" << std::endl;
	if (!(someStream.eof()))
	{
		//std::cout << "getting line pt2 " << std::endl;
		std::getline(someStream, someString);
		//std::cout << "getting line pt3 " << std::endl;
		someStmt.omniSetString(someString, t);
		//std::cout << "done " << std::endl;
	}
}

void safe_NextPopFields(HoneyDLogStatement &someStmt, std::string &someString, std::fstream &someStream, HoneyDLogStatement::t_LOGTYPE &someType)
{
	
	if (!(someStream.eof()))
	{
		std::getline(someStream, someString);
		someStmt.populateFields(someString, someType);
	}
}



int main()
{
	main_init();
	std::string bucket = "";
	std::fstream cache(configuration.cache_filePath.c_str(), std::ios::app | std::ios::in | std::ios::out);
	std::fstream honey_log(configuration.honeyd_log_filePath.c_str(), std::ios_base::in);
	std::fstream apache_log(configuration.apache_log_filePath.c_str(), std::ios_base::in);
	
	HoneyDLogStatement coolGuyJones;
	honeyd_parser myParser;
	honeyd_parser::t_LOGTYPE logtype = honeyd_parser::HONEY_D;
	unsigned long int lineNo = 0;
	unsigned long int last = 0;
	unsigned long int totalNo = 18762;
	mysqlPush mysql
	(
		"honeypot1.db.7456864.hostedresource.com",
		"honeypot1",
		"Honeypot1#",
		"honeypot1",
		3306,
		NULL,
		0,
		&cache
	);
	
	flushCache(cache, mysql);
	cache.close();
	dumpFile(configuration.cache_filePath);
	cache.open(configuration.cache_filePath.c_str(), std::ios::app | std::ios::in | std::ios::out);
	//std::cout << "IT'S WORKING: " << 
	std::cout << "Status: ";
	if(mysql.isConnected())
	{
		std::cout << "Connected." << std::endl;
	}
	else
	{
		std::cout << "Not connected." << std::endl;
	}
	std::cout << "Starting honeyd log..." << std::endl;
	while (!(honey_log.eof()))
	{
		/*
		lineNo++;
		if (((lineNo * 100) / totalNo) != last)
		{
			last = ((lineNo * 100) / totalNo);
			std::cout << last << " %" << std::endl;
		}*/
		safe_NextPopFields(myParser, logtype, bucket, honey_log);
		mysql.queue_info(logtype, myParser);
	}
	mysql.flush_queue(logtype);
	honey_log.close();
	std::cout << "... done" << std::endl;
	if (mysql.isConnected())
	dumpFile(configuration.honeyd_log_filePath);
	logtype = honeyd_parser::APACHE;
	std::cout << "Starting apache log..." << std::endl;
	lineNo = 0;
	totalNo = 365;
	while (!(apache_log.eof()))
	{
		//std::cout << lineNo << std::endl;
		/*
		lineNo++;
		if (((lineNo * 100) / totalNo) != last)
		{
			last = ((lineNo * 100) / totalNo);
			//std::cout << last << " %" << std::endl;
		}*/
		safe_NextPopFields(myParser, logtype, bucket, apache_log);
		//std::cout << "Read line... " << lineNo << std::endl;
		mysql.queue_info(logtype, myParser);
	}
	mysql.flush_queue(logtype);
	apache_log.close();
	std::cout << "... done" << std::endl;
	if (mysql.isConnected())
	dumpFile(configuration.apache_log_filePath);
	
	//std::cout << "IT'S WORKING: " << mysql.isConnected() << std::endl;
	
	std::cout << std::endl;
	mysql.close();
	//std::cout << "closed successfully?" << std::endl;
	mysql.clean();
	//std::cout << "cleaned successfully?" << std::endl;
	main_exit();
	//std::cout << "exited successfully?" << std::endl;
	std::cout << "Upload complete." << std::endl;
	/** exit status zero means everything is ok */
return 0;
}
