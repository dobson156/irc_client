#include "rlwrapper.hpp"

rlwrapper::rlwrapper()
:	history { "" }
{
	sync_modlist();
}

void rlwrapper::sync_modlist() {
	if(modlist.size() < history.size()) {
		modlist.insert(modlist.end(), history.size() - modlist.size(), boost::none);
	}
}

std::size_t rlwrapper::current_index() const {
	return index;
}

const std::string& rlwrapper::get_current() const {
	auto i=current_index();
	const auto& val=modlist[i];

	return val ? *val : history[i];
}

void rlwrapper::set_current(const std::string& val) {
	modlist[current_index()]=val;
}

void rlwrapper::revert_current() {
	modlist[current_index()]=boost::none;
}

void rlwrapper::commit_current() {
	auto cur=get_current();

	if(history.back().empty()) {
		history.back() = cur;
	}
	else {
		history.push_back(cur);
	}
	revert_current();
	history.push_back({});
	sync_modlist();
	set_position_end();
}

void rlwrapper::set_position_end() {
	index=history.empty() ? 0 : history.size()-1;
}

void rlwrapper::inc_idx() {
	++index;
	if(index>=history.size())
		index=0;
}

void rlwrapper::dec_idx() {
	if(index==0)
		index=history.size();
	--index;
}


