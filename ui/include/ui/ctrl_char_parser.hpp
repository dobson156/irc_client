
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CTRL_CHAR_PARSER_HPP
#define CTRL_CHAR_PARSER_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1

//#include <boost/spirit/include/qi_core.hpp>
//the following includes speed up compilation over the above
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_operator.hpp>

#include "ctrl_char.hpp"

namespace cons {

template<typename Iter>
std::pair<ctrl_char, Iter> parse_ctrl_char(Iter first, Iter last) {
	namespace qi=boost::spirit::qi;
	ctrl_char cht;

	qi::symbols<char, ctrl_char> ctrl_map;
	ctrl_map.add
		("\x1b\x5b\x43",             ctrl_char::arrow_right)
		("\x1b\x5b\x44",             ctrl_char::arrow_left)
		("\x1b\x5b\x31\x3b\x35\x44", ctrl_char::ctrl_arrow_left)
		("\x1b\x5b\x31\x3b\x35\x43", ctrl_char::ctrl_arrow_right)
		("\x08",                     ctrl_char::backspace)
		("\x7f",                     ctrl_char::backspace)
		("\r",                       ctrl_char::carriage_ret)
		("\n",                       ctrl_char::carriage_ret)
		;

	qi::attr_type attr;
	qi::parse(first, last, ctrl_map | attr(ctrl_char::none) , cht);
	return std::make_pair(cht, first);
}

} //namespace cons

#endif //CTRL_CHAR_PARSER_HPP
