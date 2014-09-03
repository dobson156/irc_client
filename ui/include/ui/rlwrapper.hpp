
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RLWRAPPER_HPP
#define RLWRAPPER_HPP

#include <boost/optional.hpp>

#include <string>
#include <vector>

class rlwrapper {
private:
	std::vector<std::string>                  history;
	std::vector<boost::optional<std::string>> modlist;
	std::size_t                               index;

	std::size_t current_index() const;
	void sync_modlist();
public:
	rlwrapper();

	const std::string& get_current() const;
	void set_current(const std::string& val);


	void commit_current();
	void revert_current();
	void set_position_end();

	void inc_idx();
	void dec_idx();
}; //class rlwrapper

#endif //RLWRAPPER_HPP
