#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

void handle_join(const std::vector<std::string>& input) {
	std::copy(
		input.cbegin(),
		input.cend(),
		std::ostream_iterator<std::string>(std::cout, "\n")
	);
}

void handle_part(const std::string& chan, const std::string msg) {
	std::cout << chan << " " << msg << '\n';
}

void handle_connect(const std::string& chan) {
	std::cout << chan << '\n';
}

void handle_nick(const std::string& nick) {
	std::cout << nick << '\n';
}

void handle_msg(const std::string& target, const std::string& msg) {
	std::cout << target << " " << msg << '\n';
}

struct printer
{
    typedef boost::spirit::utf8_string string;

    void element(string const& tag, string const& value, int depth) const
    {
        for (int i = 0; i < (depth*4); ++i) // indent to depth
            std::cout << ' ';

        std::cout << "tag: " << tag;
        if (value != "")
            std::cout << ", value: " << value;
        std::cout << std::endl;
    }
};

void print_info(boost::spirit::info const& what) {
    using boost::spirit::basic_info_walker;
    printer pr;
    basic_info_walker<printer> walker(pr, what.tag, 0);
    boost::apply_visitor(walker, what.value);
}


int main() {
	std::string input="/join hello bob";
	auto first=input.cbegin(),
	     last =input.cend();

	namespace qi=boost::spirit::qi;
	namespace phx=boost::phoenix;

	qi::char_type char_;
	qi::lit_type  lit;
	qi::_1_type   _1;
	qi::_1_type   _2;
	qi::_val_type _val;
	qi::string_type string;

	qi::rule<decltype(first), std::string()> str=+~char_(' ');

	try {
		qi::parse(
			first,
			last,
			lit('/') >
			( "join "   >> (str % ' ') [ phx::bind( &::handle_join,    _1 )     ]
			| "leave"   >> str >> str  [ phx::bind( &::handle_part,    _1, _2 ) ]
			| "connect" >> str         [ phx::bind( &::handle_connect, _1 )     ]
			| "nick"    >> str         [ phx::bind( &::handle_nick,    _1 )     ]
			| "msg"     >> str >> str  [ phx::bind( &::handle_msg,     _1, _2)  ]
			)
		);
	}
	catch(const qi::expectation_failure<decltype(first)> x) {
		std::cerr << "expected: "; print_info(x.what_);
		std::cerr << "got: \"" << std::string(x.first, x.last) << '"' << std::endl;
	}
	catch(const std::exception& x) {
		std::cerr << x.what() << std::endl;
	}
}
