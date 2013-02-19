#include <console.hpp>

#include <iostream>
#include <sstream>

int main() {
	try { 
		using namespace cons;
		using anchor_top   =anchor_view<anchors::top>;
		using anchor_bottom=anchor_view<anchors::bottom>;
		using anchor_left  =anchor_view<anchors::left>;
		using anchor_right =anchor_view<anchors::right>;
		using text_box     =stenciled_frame<string_stencil>;

//		anchor_top    p { make_window(), 20 };
		anchor_top top { make_window(), 20 };

		//anchor_left&  top=p.emplace_anchor<anchor_left>(20);
//		anchor_right& btm=p.emplace_fill<anchor_right>(20);

	//TOP - left
		bordered&   tlb=top.emplace_anchor<bordered>(borders::right, borders::top, borders::left, borders::bottom);
		text_box&    tlf=tlb.emplace_element<text_box>("top left");

	//TOP - right 
		text_box&    trf=top.emplace_fill<text_box>("top right");
		trf.set_background(COLOR_CYAN);
		
		/*
	//BTM - right  
		text_box&    brf=btm.emplace_anchor<text_box>("btm right");

	//BTM - left 
		text_box&    blf=btm.emplace_fill<text_box>("btm left");

		*/
		top.refresh();

		//std::ofstream debug {"debugz", std::ios_base::app };

		int i;
		while((i=getch( ))!='q') {
			switch(i) {
			case 'h':
				break;
			case 'j': {
				int n=top.get_partition();
				top.set_partition(n+1);
				//debug << n+1 << "  " << tlb.get_position() << "  " << tlf.get_position() << std::endl;
				//top.refresh();
				break;
			}
			case 'k': {
				int n=top.get_partition();
				top.set_partition(n-1);
				//debug << n-1 << "  " << tlb.get_position() << "  " << tlf.get_position() << std::endl;

				//top.refresh();
				break;
			}
			case 'l':
				break;
			default:
				break;
			}
			//trf.set_content("zip");
			{
				std::ostringstream oss;
				//oss << tlf.get_dimension();
				oss << tlb.get_position();
				tlf.set_content(oss.str());
				//tlf.refresh();
			}
			{
				std::ostringstream oss;
				//oss << trf.get_dimension();
				oss << trf.get_position();
				trf.set_content(oss.str());
				//trf.refresh();
			}

			top.refresh();
		}
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
