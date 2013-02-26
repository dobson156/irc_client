#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include <boost/spirit/home/support/info.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

void handle_join(const std::vector<std::string>& input) {
	std::cout << "JOIN\n";	
	std::copy(input.cbegin(), input.cend(),
		std::ostream_iterator<std::string>(std::cout, "\n"));
}
void handle_part(const std::string& chan, const std::string msg) {
	std::cout << "PART " << chan << " " << msg << '\n';
}
void handle_connect(const std::string& chan) {
	std::cout << "CONNECT " << chan << '\n';
}
void handle_nick(const std::string& nick) {
	std::cout << "NICK " << nick << '\n';
}
void handle_msg(const std::string& target, const std::string& msg) {
	std::cout << "MSG " << target << "\n" << msg << '\n';
}
void handle_text(const std::string& text) {
	std::cout << text << '\n';
}
void handle_exec(const std::string& exec) {
	std::system(exec.c_str());
//	std::cout << "CMD: " << exec << '\n';
}

template<typename Iter>
void parse(Iter first, Iter last) {
	namespace qi=boost::spirit::qi;
	namespace phx=boost::phoenix;

	qi::char_type   char_;
	qi::lit_type    lit;
	qi::space_type  space;
	qi::lexeme_type lexeme;

	qi::_1_type     _1;
	qi::_2_type     _2;

	//This rule IS space sensitive
	qi::rule<decltype(first), std::string()> str=+~char_(' ');

#define CMD_NM(head, tail) lexeme[ lit(head) >> -lit(tail) >> space ]
	qi::phrase_parse(
		first,
		last,
		( !lit('/') >> str                        [ phx::bind( &::handle_text,    _1 )     ]
		|'/' 
		>>  ( CMD_NM('j', "oin" ) >> (+str)       [ phx::bind( &::handle_join,    _1 )     ]
		    | CMD_NM('m', "sg"  ) >> (str >> str) [ phx::bind( &::handle_msg,     _1, _2 ) ]
		    | CMD_NM('l', "eave") >> (str >> str) [ phx::bind( &::handle_part,    _1, _2 ) ]
		    | "connect"           >> str          [ phx::bind( &::handle_connect, _1 )     ]
		    | "nick"              >> str          [ phx::bind( &::handle_nick,    _1 )     ]
		    | "exec"              >> str          [ phx::bind( &::handle_exec,    _1 )     ]
		    )
		)
		, 
		space
	);
#undef CMD_NM
}

int main() {
	std::string input="/join hello bob";

	while(std::getline(std::cin, input)) {
		parse(input.cbegin(), input.cend());
	}

	return EXIT_SUCCESS;
}
