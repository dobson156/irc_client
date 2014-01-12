
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "controller.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <csignal>

namespace po = boost::program_options;

//It's UB for a handler to not have C linkage
extern "C" {
	void handle_sigwinch(int sig) {
		assert(sig==SIGWINCH);
	}
}

int main(int argc, char **argv) {
	std::signal(SIGWINCH, handle_sigwinch);
	try {
		po::options_description desc { "Allowed options:" };
		desc.add_options()
			( "help", "produce help message" )
			( "config-script", po::value<std::string>(), 
				"specify a python config file to load instead of default" )
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if(vm.count("help")) {
			std::cout << desc << "\n";
		}
		else {
			const std::string& python_file= vm.count("config-script") > 0
			                              ? vm["config-script"].as<std::string>()
			                              : "t.py"
			                              ;

			controller ctrl { python_file };
			ctrl.run();
		}
	}
	catch(const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
