#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstdio>

void dumpFile(std::string &fpath)
{
	std::cout << "removing... " << std::endl;
	std::remove(fpath.c_str());
	system( ("touch " + fpath).c_str());
	chmod(fpath.c_str(), 
		S_IRUSR | S_IWUSR |
		S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH );
}




int main()
{
	std::string TestPath = "/home/mason/gits/lpmc/log-parser-mysql-client/parser/src/test/test.txt";
	std::fstream testStream (TestPath.c_str(), std::ios::app | std::ios::in | std::ios::out);
	std::cout << testStream.good() << std::endl;
	std::string s = "";
	while (!testStream.eof())
	{
		if (testStream.peek() != EOF)
		std::cout << (char)testStream.get();//std::getline(testStream, s);
		else
			std::cout << "eof" << std::endl;
		//std::cout << s << std::endl;
	}
	std::cout << std::endl;
	std::getline(std::cin, s);
	testStream.seekg(0);
	testStream << s << std::endl;
	std::cout << s << std::endl;
	testStream.seekg(0);
	while (!testStream.eof())
	{
		if (testStream.peek() != EOF)
		std::cout << (char)testStream.get();//std::getline(testStream, s);
		else
			std::cout << "eof" << std::endl;
		//std::cout << s << std::endl;
	}
	std::cout << std::endl;
	testStream.close();
	return 0;
}
