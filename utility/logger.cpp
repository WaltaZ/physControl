#include <utility/logger.h>
#include <iostream>

#define DEBUG

void log(const std::string& message)
{
#ifdef DEBUG
	std::cout << message << std::endl;
#endif
}