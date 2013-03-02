#include "controller.hpp"

#include <iostream>

int main() {
	try {
		controller ctrl;
		ctrl.run();
	}
	catch(const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
