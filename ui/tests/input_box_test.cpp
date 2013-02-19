#include <console.hpp>

int main() {
	using namespace cons;

	anchor_view<anchors::top> p { make_window(), 20 };

	auto& text=p.emplace_fill<stenciled_frame<string_stencil>>("tes");
	//auto& text2=p.emplace_anchor<stenciled_frame<string_stencil>>("tes");
	auto& input=p.emplace_anchor<input_box>();


	std::ostringstream oss;
	oss << text.get_dimension() << "  " << text.get_position();
	text.set_content(oss.str());
	text.set_background(COLOR_RED);

	input.set_background(COLOR_GREEN);
	//text2.set_background(COLOR_GREEN);

	p.refresh();

	while(input.get_char()!='q') {
//	while(getch()!='q') {

		p.set_partition(p.get_partition()+1);
		//p.refresh();

		//int i=p.get_partition();
//		endwin();
		//std::cerr << text.get_position() <<  text.get_dimension() << std::endl;
		//return 0;

		//p.refresh();

//		text.clear();	

		std::ostringstream oss;
		oss <<  text.get_dimension() << "  " << text.get_position() << "\nhi";
		text.set_content(oss.str());
		text.set_background(COLOR_BLUE);
		//text.refresh();

		p.refresh();
		text.set_content("hi");
		p.refresh();
	}


	/*
	input.reg_on_grow(
		[&](const point& pt) -> bool {
///			p.set_partition(pt.y);
			std::ostringstream oss;
			oss << "z\n" << text.get_dimension() << "  " << text.get_position();

			text.set_content(oss.str());
			text.set_background(COLOR_BLUE);

			p.refresh();

			return true;	
		}
	);
	*/
	//input.get_char();
}


