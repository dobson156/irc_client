#include "progress_bar.hpp"
namespace cons {

progress_bar::progress_bar(unique_window_ptr handle_,
                           unsigned progress, 
                           unsigned denominator)
:	minimal_frame_wrapper(std::move(handle_))
,	numerator_           { progress         }
,	denominator_         { denominator      }
{	}

void progress_bar::refresh() {
	const auto& dim=get_dimension();
	auto& frame_=get_frame();

	if(dim.y <= 0 || dim.x <= 0) return;

	frame_.clear();
	
	cons::point pos {0,0};

	if(is_decorated()) {
		pos=frame_.write({0,0}, '[');
		frame_.write({dim.x-1, 0}, ']');
	}
	//write a std::string with nbocks of '#'
	frame_.write(pos, std::string(get_number_of_blocks(), '#'));
	frame_.refresh();
}

unsigned progress_bar::get_number_of_blocks() const {
	//TODO catch case where x < 2
	auto x=get_frame().get_dimension().x - is_decorated() * 2;

	return x*get_progress()/get_denominator();
}

bool progress_bar::is_decorated() const { return is_decorated_; }
void progress_bar::set_decorated(bool val) { is_decorated_=val; }

unsigned progress_bar::get_progress() const { return numerator_; }
void progress_bar::set_progress(unsigned val) { numerator_=val; }

unsigned progress_bar::get_denominator() const { return denominator_; }
void progress_bar::set_denominator(unsigned val) { denominator_=val; }

} //namespace cons
