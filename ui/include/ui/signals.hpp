
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <boost/signals2.hpp>

#include <string>
#include "basic.hpp"
#include "ctrl_char.hpp"

namespace cons {
	namespace bsig=boost::signals2;

	using sig_pt=bsig::signal<bool(point)>;
	using sig_s =bsig::signal<void(const std::string&)>;
	using sig_ctrl_ch=bsig::signal<void(ctrl_char)>;
} //namespace cons

#endif //SIGNALS_HPP
