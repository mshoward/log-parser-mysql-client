#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>


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
	std::string s = "";
	std::getline(std::cin, s);
	dumpFile(s);
	return 0;
}
