#pragma once
#define NUMBER_OF_SENT 5


class mysqlPush
{
	
	
	public:
	
	bool debug;
	void prepStmt(HoneyDLogStatement *params);
	std::fstream *backup;
	std::queue<HoneyDLogStatement> logQueue;
	unsigned int logsWaiting;
	unsigned int flushAt;
	MYSQL *mysql;
	MYSQL_STMT *stmt;
	MYSQL_BIND binds[7 * NUMBER_OF_SENT];
	MYSQL_BIND single[7];
	long unsigned int lens[7 * NUMBER_OF_SENT];
	long unsigned int singleLen;
	unsigned int err;
	//MYSQL_STMT *stmt2;
	std::string query;
	//char queryString[];
	std::string query2;
	std::string honeyd_query;
	std::string apache_query;
	void updateErr();
	void dumperr();
	//std::string dumpToFile;
	//std::string dumpToFile2;
	
	//enum t_LOGTYPE {HONEY_D, APACHE, OTHER_LOG};
	int queue_info(hdparser::honeyd_parser::t_LOGTYPE which, hdparser::honeyd_parser &parsed);
	int flush_queue(hdparser::honeyd_parser::t_LOGTYPE which);
	enum MODE {TO_SERVER, TO_FILE, OTHER};
	
	MODE mode;
	/** is broken */
	mysqlPush();
	
	/** is magic -- sets up everything, including starting the connection,
	 * statement inits, fstream checking -- the works 
	 */
	mysqlPush
	(
		const char *host,
		const char *user, 
		const char *passwd,
		const char *db, 
		unsigned int port, 
		const char *unix_socket,
		unsigned long client_flag, 
		std::fstream *backUpFileStream
	);
	~mysqlPush()
	{
		clean();
	}
	
	
	
	/**
	 * this is the one you should call -- it'll pick the correct func to call.
	 * the others are / will be self monitoring and will correct the state
	 * should a disconnection occur, or an i/o error happen.  if an i/o
	 * error happens and a disconnect, the program will probably terminate.
	 */
	int update(HoneyDLogStatement *params);
	int updateServer(HoneyDLogStatement *params);
	int updateFile(HoneyDLogStatement *params);
	int execQuery(std::string &someString);
	int enqueue(HoneyDLogStatement &param);
	
	/**returns true if connection to the server is good, or was reestabished
	 * by pinging
	 * also sets mode dependent on status
	 */
	bool isConnected();
	bool close();
	/**
	 * lets you know whether something's broken.  One could also check this->mode
	 */
	bool isGood();
	/**
	 * memory management -- should be called when done with things (everything)
	 */
	bool clean();
	//MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd,
	// const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag)
};
