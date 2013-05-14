#include "../ui/console.hpp"

#include "window_stencil.hpp"
#include "window.hpp"

#include <iostream>
#include <sstream>

int main() {
	try { 
		using namespace cons;
		using window_frame=stenciled_frame<window_stencil>;

		window w;

		window_frame wf { make_window(), w };
		wf.refresh();
		getch();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
