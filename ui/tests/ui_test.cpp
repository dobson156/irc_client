#include "ui_irc_window.hpp"

#include <exception>
#include <iostream>
#include <cstdlib>

int main() {
	try {
		irc_ui_window iuw;
		iuw.run();
	}
	catch(const std::exception& e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
