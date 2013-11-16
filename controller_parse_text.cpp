#include "controller.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>


/*
** This function takes a month of sundays to compile 
** hence it has it's own TU
*/
void controller::parse_text(std::string::const_iterator first,
                            std::string::const_iterator last) {
	//namespace alias
	namespace qi=boost::spirit::qi;
	namespace phx=boost::phoenix;
	//qi types
	qi::char_type   char_;
	qi::lit_type    lit;
	qi::space_type  space;
	qi::lexeme_type lexeme;
	qi::_1_type     _1;
	qi::_2_type     _2;

	//This rule IS space sensitive
	qi::rule<decltype(first), std::string()> str=+~char_(' ');

	qi::rule<decltype(first), std::string()> str_to_end=+char_;

	auto cmd_nm=[&](char head, const char *tail) {
		//TODO: replace with qi::copy as and when it becomes available
		return boost::proto::deep_copy(lexeme [ lit(head) >> -lit(tail) >> space ]);
	};

	qi::phrase_parse(
		first,
		last,
		( !lit('/') >> str_to_end                 [ phx::bind( &::controller::handle_text,    this, _1 )     ]
		|'/' 
		>>  ( cmd_nm('j', "oin" ) >> (+str)       [ phx::bind( &::controller::handle_join,    this, _1 )     ]
			| cmd_nm('m', "sg"  ) >> (str >> str) [ phx::bind( &::controller::handle_msg,     this, _1, _2 ) ]
			//| cmd_nm('n', "ames")                 [ phx::bind( &::controller::handle_names,   this)          ]
			| cmd_nm('l', "eave") >> (str >> str) [ phx::bind( &::controller::handle_part,    this, _1, _2 ) ]
			| "connect"           >> str          [ phx::bind( &::controller::handle_connect, this, _1 )     ]
			| "nick"              >> str          [ phx::bind( &::controller::handle_nick,    this, _1 )     ]
			| "exec"              >> str          [ phx::bind( &::controller::handle_exec,    this, _1 )     ]
			| lit("quit")                         [ phx::bind( &::controller::handle_quit,    this)          ]
			| lit("names")                        [ phx::bind( &::controller::handle_names,   this)          ]
			)
		)
		, 
		space
	);
}
