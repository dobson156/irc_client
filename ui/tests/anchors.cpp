#include <console.hpp>

#include <iostream>
#include <sstream>

int main() {
	try { 
		using anchor_top   =cons::anchor_view<cons::anchors::top>;
		using anchor_bottom=cons::anchor_view<cons::anchors::bottom>;
		using anchor_left  =cons::anchor_view<cons::anchors::left>;
		using anchor_right =cons::anchor_view<cons::anchors::right>;
		using text_box     =cons::stenciled_frame<cons::string_stencil>;
		using str_list     =cons::stenciled_list<cons::string_stencil>;

		anchor_top    p { cons::make_window(), 20 };

		anchor_left&  top=p.emplace_anchor<anchor_left>(20);
		anchor_right& btm=p.emplace_fill<anchor_right>(20);

	//TOP - left
		cons::bordered&   tlb=top.emplace_anchor<cons::bordered>(
			cons::borders::right, cons::borders::left, 
			cons::borders::top,   cons::borders::bottom);
		text_box&    tlf=tlb.emplace_element<text_box>("top left");
		tlf.set_background(COLOR_YELLOW);

	//TOP - right 
		cons::bordered&   trb=top.emplace_fill<cons::bordered>(
			cons::borders::right, cons::borders::left, cons::borders::top, cons::borders::bottom);
		text_box&    trf=trb.emplace_element<text_box>("top right");
		trf.set_background(COLOR_BLUE);
		
	//BTM - right  
		cons::bordered&   brb=btm.emplace_anchor<cons::bordered>(
			cons::borders::right, cons::borders::left, cons::borders::top, cons::borders::bottom);
		text_box&    brf=brb.emplace_element<text_box>("btm right");
		brf.set_background(COLOR_RED);

	//BTM - left 
		cons::bordered&   blb=btm.emplace_fill<cons::bordered>(
			cons::borders::right, cons::borders::left, cons::borders::top, cons::borders::bottom);
		str_list&    blf=blb.emplace_element<str_list>();
		//blf.set_background(COLOR_GREEN);

		blf.insert(blf.begin(), "hello");
		blf.insert(blf.begin(), "world");

		p.refresh();

		int i;
		while((i=getch())!='q') {
			switch(i) {
			case 'h': {
				int n=p.get_partition();
				p.set_partition(n+1);
				p.refresh();
				break;
			}
			case 'l': {
				int n=p.get_partition();
				p.set_partition(n-1);
				p.refresh();
				break;
			}
			case 'j': {
				int n=top.get_partition();
				top.set_partition(n+1);
				top.refresh();
				break;
			}
			case 'k': {
				int n=top.get_partition();
				top.set_partition(n-1);
				top.refresh();
				break;
			}
			default:
				break;
			}
		}
	} catch(const std::exception& e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
	}
}
