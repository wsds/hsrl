#include <iostream>
#include "test.h"
#include "thrift\gen-cpp\shell_server.skeleton.h"


//Entry
void testEntry()
{

	std::cout << "test entry." << std::endl;
	server_main(0,0);
	//test4();
	std::cout << "test end." << std::endl;

}
