#include "../parser-includes.hpp"




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
	std::cout << "didn't segfault yet " << std::endl;
	mysql_library_end();
	//mysql_library_end();
}




int main()
{
	main_init();
	char *s = "foo2";
	unsigned long int lens = 5;
	char *s2 = "bar2";
	mysqlPush mysql
	(
		"honeypot1.db.7456864.hostedresource.com",
		"honeypot1",
		"Honeypot1#",
		"honeypot1",
		3306,
		NULL,
		0,
		NULL
	);
	MYSQL_BIND binds[2];
	memset(binds, 0, sizeof(binds));
	MYSQL_STMT * stmt = mysql_stmt_init(mysql.mysql);
	mysql_stmt_prepare(stmt,
	"INSERT INTO 'honeypot1'.'testTable' ('col1','col2', 'col3') VALUES ( NULL, ?, ?)",
	81);
	//std::cout << "didn't segfault 3" << std::endl;
	binds[0].buffer_type = MYSQL_TYPE_STRING;
	binds[0].buffer = s;
	binds[0].buffer_length = lens;
	//std::cout << "didn't segfault 4" << std::endl;
	//lens[0] = (params->timeStamp.length());
	binds[0].length = &lens;
	//std::cout << "didn't segfault 5" << std::endl;
	binds[0].is_null = (my_bool *) 0;
	//std::cout << "didn't segfault 6" << std::endl;
	//std::cout << "didn't segfault 3" << std::endl;
	
	binds[1].buffer_type = MYSQL_TYPE_STRING;
	binds[1].buffer = s2;
	binds[1].buffer_length = lens;
	//std::cout << "didn't segfault 4" << std::endl;
	//lens[0] = (params->timeStamp.length());
	binds[1].length = &lens;
	//std::cout << "didn't segfault 5" << std::endl;
	binds[1].is_null = (my_bool *) 0;
	mysql_stmt_bind_param(stmt, binds);
	mysql_stmt_execute(stmt);
	std::cout << mysql_stmt_error(stmt) << std::endl;
	
	mysql_stmt_close(stmt);
	mysql.close();
	mysql.clean();
	main_exit();
}
