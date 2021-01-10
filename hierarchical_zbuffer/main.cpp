#include <direct.h>

#include <iostream>
#include <stdexcept>

#include "application.h"

/* program entry point */
int main() {
	char buf[1024];
	_getcwd(buf, 1024);
	std::cout << "current directory: " << buf << std::endl;

	try {
		Application app;
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}