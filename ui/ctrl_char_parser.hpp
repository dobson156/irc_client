#ifndef CTRL_CHAR_PARSER_HPP
#define CTRL_CHAR_PARSER_HPP

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS 1

//#include <boost/spirit/include/qi_core.hpp>
//the following includes speed up compilation over the above
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_operator.hpp>

enum class ctrl_char {
	none, arrow_left, arrow_right, backspace
};

template<typename Iter>
std::pair<ctrl_char, Iter> parse_ctrl_char(Iter first, Iter last) {
	namespace qi=boost::spirit::qi;
	ctrl_char cht;

	qi::symbols<char, ctrl_char> ctrl_map;
	ctrl_map.add
		("\x1b\x5b\x43", ctrl_char::arrow_right)
		("\x1b\x5b\x44", ctrl_char::arrow_left)
		("\x08",         ctrl_char::backspace)
		("\x7f",         ctrl_char::backspace)
		;

	qi::attr_type attr;
	qi::parse(first, last, ctrl_map | attr(ctrl_char::none) , cht);
	return std::make_pair(cht, first);
}

#endif //CTRL_CHAR_PARSER_HPP