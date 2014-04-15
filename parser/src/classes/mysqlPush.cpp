#pragma once


/**
 * don't use -- is trash and pointless to have
 * if you accidentally use it, deallocate immediately and call the other constructor
 */
mysqlPush::mysqlPush()
{
	//don't use
	mode = OTHER;
	backup = NULL;
	mysql = NULL;
	stmt = NULL;
	query = "";
	
}



/**
 * starts the mysql server connection
 * sets up all the things
 * inits the prepared sql statements
 * sets output mode
 * is magic
 */
mysqlPush::mysqlPush(const char *host, const char *user, const char *passwd,
			const char *db, unsigned int port, const char *unix_socket,
			unsigned long client_flag, std::fstream *backUpFileStream)
{
	err = 0;
	debug = false;
	//init mysql stuff
	my_init();
	stmt = 0;
	logsWaiting = 0;
	flushAt = 1000;
	memset(binds, 0, sizeof(binds));
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, host, user, passwd, db, port, unix_socket, client_flag);
	updateErr();
	backup = backUpFileStream;
	query = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version) VALUES (?, ?, ?, ?, ?, ?, ?)";
	//queryString = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version) VALUES (?, ?, ?, ?, ?, ?, ?)";
	query2 = "THIS IS NOT THE OTHER QUERY";
	if (mysql_ping(mysql) != 0)
	{
		if (backup->is_open())
			mode = TO_FILE;
		else
			mode = OTHER;
	}
	else
		mode = TO_SERVER;
	updateErr();
	stmt = mysql_stmt_init(mysql);
	updateErr();
	dumperr();
	//binds = 0;
	//stmt2 = mysql_stmt_init(mysql);
	mysql_stmt_prepare(stmt, 
	"INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version) VALUES (?, ?, ?, ?, ?, ?, ?)", query.length());
	updateErr();
	dumperr();
	honeyd_query = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version,  attackType, apacheTime) VALUES ";
	apache_query = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version,  attackType, apacheTime) VALUES ";
	//mysql_stmt_prepare(stmt2, query2.c_str(), query2.length());
}

int mysqlPush::queue_info(hdparser::honeyd_parser::t_LOGTYPE which, hdparser::honeyd_parser &parsed)
{
	int ret = 0;
	bool val;
	if (which == hdparser::honeyd_parser::HONEY_D)
		val = parsed.getValidOsVersion() &&
				parsed.getValidPacketType() &&
				parsed.getValidSourceIP() &&
				parsed.getValidSourceSocket() &&
				parsed.getValidTargetIP() &&
				parsed.getValidTargetSocket() &&
				parsed.getValidTimeStamp();
	else
		val = parsed.getValidTimeStamp()&&
				parsed.getValidSourceIP();
	
	std::string temp = "\n(";
	if (logsWaiting > 0)
		temp = ", (";
	if ((which == hdparser::honeyd_parser::HONEY_D) && val)
	{
		temp += "\"" + parsed.timeStamp + "\", ";
		temp += "\"" + parsed.packetType + "\", ";
		temp += "\"" + parsed.sourceIP + "\", ";
		temp += "\"" + parsed.sourceSocket + "\", ";
		temp += "\"" + parsed.targetIP + "\", ";
		temp += "\"" + parsed.targetSocket + "\", ";
		temp += "\"";
		//std::cout << "error" << std::endl;
		if (parsed.osVersion.length() > 0)
		temp += (parsed.osVersion).substr(1, parsed.osVersion.length() - 2);
		//std::cout << "error" << std::endl;
		temp += "\", ";
		temp += "NULL, ";
		temp += "NULL)";
		temp += "\n";
		honeyd_query += temp;
		//std::cout << temp << std::endl;
	}
	else if (val)
	{
		//"INSERT INTO Information 
		//(DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version,  attackType, apacheTime) VALUES ";
		temp += "\"" + parsed.timeStamp  + "\"" + ", ";
		temp += "NULL, ";
		temp += "\"" + parsed.sourceIP + "\"" + ", ";
		temp += "NULL, ";
		temp += "NULL, ";
		temp += "NULL, ";
		temp += "NULL, ";
		temp += "\"" + parsed.attackStr + "\"" + ", ";
		temp += "\"" + parsed.timeStamp + "\"" + ")\n";
		//std::cout << temp << std::endl;
		apache_query += temp;
	}
	if (val)
	logsWaiting++;
	//std::cout << logsWaiting << std::endl;
	if(logsWaiting >= flushAt)
	{
		ret = (flush_queue(which));
		if(ret != 0)
		{
			std::cout << "cannot update server" << std::endl;
			return ret;
		}
		else
		{
			logsWaiting = 0;
			return ret;
		}
	}
	else
	{
		return ret;
	}
}

int mysqlPush::flush_queue(hdparser::honeyd_parser::t_LOGTYPE which)
{
	int ret;
	if(which == hdparser::honeyd_parser::HONEY_D)
	{
		//std::cout << "SENDING DATA..." << std::endl;
		honeyd_query += ";";
		//std::cout << honeyd_query << std::endl;
		ret = execQuery(honeyd_query);
		logsWaiting = 0;
		honeyd_query = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version,  attackType, apacheTime) VALUES ";
		return ret;
	}
	else
	{
		std::cout << "SENDING DATA..." << std::endl;
		apache_query += ";";
		//std::cout << apache_query << std::endl;
		ret = execQuery(apache_query);
		logsWaiting = 0;
		apache_query = "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version,  attackType, apacheTime) VALUES ";
		return ret;
	}
}

/** Will determine the correct function to call based on current state
 */
int mysqlPush::update(HoneyDLogStatement *params)
{
	int ret = 1;
	isConnected();
	if (mode == TO_SERVER)
		ret = updateServer(params);
	
	if ((ret != 0) && (mode != OTHER))
		return updateFile(params);
	if (mode == OTHER)
		return 1;
}

void mysqlPush::prepStmt(HoneyDLogStatement *params)
{
	unsigned int startingPoint = 7 * logsWaiting;
	//std::cout << "didn't segfault 1" << std::endl;
	/*if (binds != (MYSQL_BIND *) 0)
	{
		free(binds);
		binds = 0;
	}*/
	//long unsigned int lens[7];
	//std::cout << "didn't segfault 2" << std::endl;
	//binds = (MYSQL_BIND *) malloc(sizeof(MYSQL_BIND) * 7);
	
	
	//std::cout << "didn't segfault 3" << std::endl;
	binds[startingPoint + 0].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 0].buffer = &(params->timeStamp[0]);
	binds[startingPoint + 0].buffer_length = params->timeStamp.length();
	//std::cout << "didn't segfault 4" << std::endl;
	lens[startingPoint + 0] = (params->timeStamp.length());
	binds[startingPoint + 0].length = &lens[0];
	//std::cout << "didn't segfault 5" << std::endl;
	binds[startingPoint + 0].is_null = (my_bool *) 0;
	//std::cout << "didn't segfault 6" << std::endl;
	/**
	 * finish the rest of them
	 */
	binds[startingPoint + 1].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 1].buffer = &(params->packetType[0]);
	binds[startingPoint + 1].buffer_length = params->packetType.length();
	lens[startingPoint + 1] = (params->packetType.length());
	binds[startingPoint + 1].length = &lens[1];
	binds[startingPoint + 1].is_null = (my_bool *) 0;
	
	binds[startingPoint + 2].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 2].buffer = &(params->sourceIP[0]);
	binds[startingPoint + 2].buffer_length = params->sourceIP.length();
	lens[startingPoint + 2] = (params->sourceIP.length());
	binds[startingPoint + 2].length = &lens[2];
	binds[startingPoint + 2].is_null = (my_bool *) 0;
	
	binds[startingPoint + 3].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 3].buffer = &(params->sourceSocket[0]);
	binds[startingPoint + 3].buffer_length = params->sourceSocket.length();
	lens[startingPoint + 3] = (params->sourceSocket.length());
	binds[startingPoint + 3].length = &lens[3];
	binds[startingPoint + 3].is_null = (my_bool *) 0;
	
	binds[startingPoint + 4].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 4].buffer = &(params->targetIP[0]);
	binds[startingPoint + 4].buffer_length = params->targetIP.length();
	lens[startingPoint + 4] = (params->targetIP.length());
	binds[startingPoint + 4].length = &lens[4];
	binds[startingPoint + 4].is_null = (my_bool *) 0;
	
	binds[startingPoint + 5].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 5].buffer = &(params->targetSocket[0]);
	binds[startingPoint + 5].buffer_length = params->targetSocket.length();
	lens[startingPoint + 5] = (params->targetSocket.length());
	binds[startingPoint + 5].length = &lens[5];
	binds[startingPoint + 5].is_null = (my_bool *) 0;
	
	binds[startingPoint + 6].buffer_type = MYSQL_TYPE_STRING;
	binds[startingPoint + 6].buffer = &(params->osVersion[0]);
	binds[startingPoint + 6].buffer_length = params->osVersion.length();
	lens[startingPoint + 6] = (params->osVersion.length());
	binds[startingPoint + 6].length = &lens[6];
	binds[startingPoint + 6].is_null = (my_bool *) 0;//binds[0].buffer_type = MYSQL_TYPE_STRING;
	
	//mysql_stmt_bind_param(stmt, binds);
	updateErr();
}


int mysqlPush::updateServer(HoneyDLogStatement *params)
{
	//(DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version)
	prepStmt(params);
	dumperr();
	if (isConnected())
	{
		mysql_stmt_bind_param(stmt, binds);
		updateErr();
		dumperr();
		//std::cout << "CALLING EXECUTE" << std::endl;
		mysql_stmt_execute(stmt);
		//std::cout << "SUCCESS" << std::endl;
		updateErr();
		dumperr();
		if (err)
		{
			
			exit(4);
		}
		return 0;
	}
	else
		return 1;
	
}

void mysqlPush::updateErr()
{
	err = err | mysql_errno(mysql);
}

void mysqlPush::dumperr()
{
	if (err)
	{
		std::cout << "should be 0 >> " << err << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
		std::cout << mysql_stmt_error(stmt) << std::endl;
		//std::cout << mysql_stmt_param_count(stmt) << std::endl;
		std::cout << stmt << std::endl;
	}
}

bool mysqlPush::isConnected()
{
	if (mysql_ping(mysql) != 0)
	{
		if (backup->is_open())
		{
			mode = TO_FILE;
			return false;
		}
		else
		{
			mode = OTHER;
			return false;
		}
	}
	else
	{
		mode = TO_SERVER;
		return true;
	}
}

/**
 * need to check for whether or not stream is good to go before shoving things at it
 */
int mysqlPush::updateFile(HoneyDLogStatement *params)
{
	
	(*backup) << "INSERT INTO Information (DateTime, packetType, sourceIP, sourceSocket, targetIP, targetSocket, Version) VALUES ( '";
	(*backup) << params->timeStamp << "', '" << params->packetType << "', '" << params->sourceIP << "', '";
	(*backup) << params->sourceSocket << "', '" << params->targetIP << "', '" << params->targetSocket << "', '" << params->osVersion << "');" << std::endl;
	return 0;
}

bool mysqlPush::close()
{
	if (debug)
	{
		std::cout << "msqlPush::close() called" << std::endl;
		std::cout << "closing connection..." << std::endl;
	}
	mysql_close(mysql);
	 if (debug) std::cout << "done" << std::endl;
	/*if (binds != (MYSQL_BIND *) 0)
	{
		std::cout << "freeing binds" << std::endl;
		free(binds);
		binds = 0;
		std::cout << "done" << std::endl;
	}*/
	if (stmt != (MYSQL_STMT *) 0)
	{
		//std::cout << "freeing stmt" << std::endl;
		mysql_stmt_close(stmt);
		stmt = 0;
		//std::cout << "done" << std::endl;
	}
	/*
	if (stmt2 != (MYSQL_STMT *) 0)
	{
		std::cout << "freeing stmt2" << std::endl;
		mysql_stmt_close(stmt2);
		stmt2 = 0;
		std::cout << "done" << std::endl;
	}
	*/
	//std::cout << "done" << std::endl;
	return true;
}

bool mysqlPush::clean()
{
	//mysql_close(mysql);
	//std::cout << "didn't segfault 1" << std::endl;
	/*if (binds != (MYSQL_BIND *) 0)
	{
		free(binds);
		binds = 0;
	}*/
	//std::cout << "didn't segfault 2" << std::endl;
	if (stmt != (MYSQL_STMT *) 0)
	{
		mysql_stmt_close(stmt);
		stmt = 0;
	}
	//std::cout << "didn't segfault 3" << std::endl;
	/*
	if (stmt2 != (MYSQL_STMT *) 0)
	{
		mysql_stmt_close(stmt2);
		stmt2 = 0;
	}
	*/
	//std::cout << "mysqlPush::clean() no segfault" << std::endl;
	return true;
}

int mysqlPush::enqueue(HoneyDLogStatement &param)
{
	logQueue.push(param);
	logsWaiting++;
	//prepStmt(&param);
}


int mysqlPush::execQuery(std::string &someString)
{
	if (isConnected())
	return mysql_query(mysql, someString.c_str());
	else
	{
		(*backup) << someString << std::endl;
	}
}
