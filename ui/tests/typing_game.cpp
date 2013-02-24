#include <console.hpp>

#include <boost/algorithm/string.hpp> //starts_with

#include <chrono>
#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
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
		for(std::size_t i=0; i!=n; ++i) {
			if(i) oss << ' ';
			oss << dict[dist(eng)];
		}
		return oss.str();
	}
};

struct score { 
	std::size_t n; 
	double      t; 
};
struct score_stencil {
	using value_type=score;
	int require_y(int, const score&) const { return 3; };
	cons::point write_to(cons::frame& frame_, const score& sc) {
		auto dim=frame_.get_dimension();
		int ts=std::min(dim.y, 3);
		switch(ts) {
		case 3: frame_.write({0,2},"words: "+std::to_string(sc.n));
		case 2: frame_.write({0,1},"time:  "+std::to_string(sc.t));
		case 1: frame_.write({0,0},"wpm:   "+std::to_string(sc.n*60/sc.t));
		}
		return { dim.y, ts };
	}
};

int main() {
	const std::string dict_path="/usr/share/dict/cracklib-small";
	std::ifstream file { dict_path };
	stentence_maker sm(file);

	using text_box  =cons::stenciled_frame<cons::string_stencil>;
	using score_box =cons::stenciled_frame<score_stencil>;
	using anchor_btm=cons::anchor_view<cons::anchors::bottom>;

	anchor_btm anc { cons::make_window(), 1 };
	auto& inp=anc.emplace_anchor<cons::input_box>();

	inp.reg_on_grow(
		[&anc](cons::point pt) {
			anc.set_partition(pt.y);
			anc.refresh();
			return true;
		}
	);

	int i=0;
	while(i!='q') {
		auto& tbx=anc.emplace_fill<text_box>("");
		std::size_t n=3;
		const auto& word=sm(n);

		tbx.set_content(word);
		inp.clear();
		anc.refresh();

		auto tstart=std::chrono::system_clock::now();
		do {
			inp.get_char();
			inp.set_foreground(boost::starts_with(word, inp.get_value())
			                 ? COLOR_GREEN : COLOR_RED);
			inp.refresh();
		} while(inp.get_value()!=word);
		auto t=std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now()-tstart).count();

		inp.clear();
		anc.emplace_fill<score_box>(score{ n, double(t)/1000 });
		anc.refresh();
		i=inp.get_char();
	}
}
