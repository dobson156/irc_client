#include <console.hpp>

#include <iterator>
#include <algorithm>

int main() {
	boost::asio::io_service io;
	auto im=cons::make_tty_manager(io);
	im.async_read(
		[](const std::string& s) {
			std::cout << std::hex;
			std::copy(
				s.begin(), s.end(),
				std::ostream_iterator<int>(std::cout, "  ")
			);
			std::cout << std::endl;
		}
	);
	io.run();
}
