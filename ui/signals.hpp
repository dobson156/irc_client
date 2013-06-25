#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <boost/signals.hpp>

#include <string>
#include "basic.hpp"
#include "ctrl_char.hpp"

namespace cons {
	namespace bsig=boost::signals;

	using sig_pt=boost::signal<bool(point)>;
	using sig_s =boost::signal<void(const std::string&)>;
	using sig_ctrl_ch=boost::signal<void(ctrl_char)>;
} //namespace cons

#endif //SIGNALS_HPP
