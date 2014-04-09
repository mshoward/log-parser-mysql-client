#!/bin/bash
g++ parser.cpp -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lssl -lcrypto -ldl -L/usr/lib64/ -o log-parser-mysql-client
