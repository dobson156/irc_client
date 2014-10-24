#ifndef CONS_PROGRESS_BAR_HPP
#define CONS_PROGRESS_BAR_HPP

#include "minimal_frame_wrapper.hpp"

namespace cons {

class progress_bar : public minimal_frame_wrapper {
private:
	bool     is_decorated_ { true };
	unsigned numerator_;
	unsigned denominator_;
	unsigned get_number_of_blocks() const;
public:
	progress_bar(unique_window_ptr handle_, 
	             unsigned progress=0, 
	             unsigned denominator=100);
	/**
	 * a decorated progress bar is a one which has buffers at either end, ie:
	 * [####     ] is decorated where as ####     is not
	 *
	 * @returns whether or not the progress bar is decorated
	 */
	bool is_decorated() const;
	/**
	 * @param sets whether or not the progress bar is decorated 
	 */
	void set_decorated(bool val);
	/**
	 * @returns the current progress value
	 */
	unsigned get_progress() const;
	/**
	 * this sets the current progress value; value should be between 1 and the denominator (defaultly 100)
	 */
	void set_progress(unsigned val);
	/**
	 * the denominator is the granularity of the progress bar:
	 * by default this is 100, so that and progress bar is effecticvely a percentage
	 * however this can be change to reflect whatever value you wish
	 * 
	 * @returns the current denominator
	 */
	unsigned get_denominator() const;
	/**
	 * Sets the denominator value
	 * 
	 * @param the value
	 */
	void set_denominator(unsigned val);
	/**
	 * draws the progress bar and outputs to screen, this intercepts the 
	 * event in minimal_frame_wrapper and calls refresh on frame itself
	 */
	void refresh() override; //from miminal_frame_wrapper
}; //class progress_bar

} //namespace cons

#endif //CONS_PROGRESS_BAR_HPP
