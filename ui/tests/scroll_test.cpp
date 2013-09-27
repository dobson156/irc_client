#include <console.hpp>

int main() {
	using namespace cons;
	bordered b {
		make_window(),
		borders::top, borders::bottom, borders::left, borders::right
	};

	auto& list=b.emplace_element<stenciled_list<string_stencil>>( ); 


	for(int i=0; i!=999; ++i) {
		list.insert(list.end(), std::to_string(i));
	}
	list.selected_idx(500);

	b.refresh();

	char c='\0';
	while(c!='q') {
		if(c=='k') {
			list.selected_idx(list.selected_idx() - 1);
			///list.selected_idx(999);
		}
		else if (c=='j') {
			list.selected_idx(list.selected_idx() + 1);
		}

		list.refresh();

		c=getch();
	}

}
