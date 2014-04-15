#pragma once
#include <string>
#include <cctype>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <queue>


//don't touch the order of these
/* when I implement the mysql client, I might end up having
 * to bring in the whole library and include them here in a sub-directory
 * to make things easier on myself
 **/

//include libraries
#include <mysql.h>
#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>




//includes headers

#include "includes/parser-sqli.hpp"
#include "includes/honeyd_parser.hpp"
#include "includes/honeyD_log.hpp"
#include "includes/mysqlPush.hpp" //depends on honeyD_log.hpp
#include "includes/parser.hpp"

//classes definitions after

#include "classes/honeyd_parser.cpp"
#include "classes/honeyD_log.cpp"
#include "classes/mysqlPush.cpp"






