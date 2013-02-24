#include <console.hpp>

#include <boost/algorithm/string.hpp> //starts_with

#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>

class stentence_maker {
	std::vector<std::string> dict;
	using isi=std::istream_iterator<std::string>;
	std::mt19937 eng { std::random_device()() };
	std::uniform_int_distribution<std::size_t> dist { 0, dict.size()-1 };
public:
	stentence_maker(std::istream& is)
	:	dict { isi(is), isi()         }
	,	eng  { std::random_device()() }
	,	dist { 0, dict.size()-1       }
	{   }

	std::string operator()(std::size_t n) {
		std::ostringstream oss;
		for(std::size_t i=0; i!=n; ++i) 
			oss << dict[dist(eng)] << ' ';
		return oss.str();
	}
};

int main() {
	const std::string dict_path="/usr/share/dict/cracklib-small";
	std::ifstream file { dict_path };
	stentence_maker sm(file);

	using text_box  =cons::stenciled_frame<cons::string_stencil>;
	using anchor_btm=cons::anchor_view<cons::anchors::bottom>;

	anchor_btm anc { cons::make_window(), 1 };
	auto& tbx=anc.emplace_fill<text_box>("");
	auto& inp=anc.emplace_anchor<cons::input_box>();

	inp.reg_on_grow(
		[&anc](cons::point pt) {
			anc.set_partition(pt.y);
			anc.refresh();
			return true;
		}
	);

	while(true) {
		const auto& word=sm(60);

		tbx.set_content(word);
		inp.clear();
		anc.refresh();

		while(inp.get_char()!='\n') {
			inp.set_foreground(boost::starts_with(word, inp.get_value())
			                 ? COLOR_GREEN
			                 : COLOR_RED);
			inp.refresh();
		}
	}
}
