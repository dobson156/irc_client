#include <console.hpp>

#include <boost/asio.hpp>

int main() {
	using namespace cons;

	anchor_view<anchors::top> p { make_window(), 20 };

	boost::asio::io_service io_service;

	auto& text=p.emplace_fill<stenciled_frame<string_stencil>>("tes");
	//auto& text2=p.emplace_anchor<stenciled_frame<string_stencil>>("tes");
	auto& input=p.emplace_anchor<async_input_box>(io_service);


	std::ostringstream oss;
	oss << text.get_dimension() << "  " << text.get_position();
	text.set_content(oss.str());
	text.set_background(COLOR_RED);

	input.set_background(COLOR_GREEN);
	//text2.set_background(COLOR_GREEN);
	//input.setup();

	input.reg_on_input([&](std::string str) {
		text.set_content(str);
		text.refresh();
	});

	p.refresh();
	io_service.run();
}


