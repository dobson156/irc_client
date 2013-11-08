#include "colour_pair.hpp"
#include "exception.hpp"
#include "assert.hpp"

#include <ncurses.h>

#include <algorithm>
#include <sstream>

namespace cons {

std::vector<unsigned> colour_pair::slot_counts;

short colour_pair::find_slot_with_colour(short fg, short bg) {
	short ft, bt;
	for(short i=0; i < short(slot_counts.size()); ++i) {
		pair_content(i+1, &ft, &bt);
		if(ft==fg && bg==bt) return i;
	}
	return -1; //colour not found
}

short colour_pair::get_first_free_slot() {
	auto it=std::find(begin(slot_counts), end(slot_counts), 0);

	if(it!=end(slot_counts))
		return std::distance(begin(slot_counts), it);

	if(short(slot_counts.size()) >= COLOR_PAIRS) {
		std::ostringstream oss;
		oss << "failed to aquire new slot, number of existing slots: " 
		    << slot_counts.size() << " maximum number of slots "
			<< COLOR_PAIRS;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	slot_counts.push_back(0);

	return slot_counts.size() -1;
}

short colour_pair::get_any_id(short fg, short bg) {
	short slot=find_slot_with_colour(fg, bg);
	if(slot==-1) { //the pair does not currently have a slot
		slot=get_first_free_slot();
	}
	++slot_counts[slot];
	++slot; //now the "id"
	init_pair_with_id(slot, fg, bg);
	return slot;
}

void colour_pair::init_pair_with_id(short id, short fg, short bg) {
	CONS_ASSERT(id != 0 && id <= COLOR_PAIRS, "invalid colour id");
	if(init_pair(id, fg, bg)==ERR) {
		std::ostringstream oss;
		oss << "Unable to set colour pair id " << id 
		    << " with fg: " << fg << " bg: " << bg;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
}

void colour_pair::release_idx(short id) {
	CONS_ASSERT(id >= 0 && id < short(slot_counts.size()), 
		"can not release slot at invalid idx");
	CONS_ASSERT(slot_counts[id] != 0u, "new slot count is out of range");
	--slot_counts[id];
}


colour_pair::colour_pair()
{
	//see man use_default_colors for details
 	use_default_colors();
	id=get_any_id(-1, -1); //-1, -1 are default colours
}
colour_pair::colour_pair(short foreground, short background)
:	id { get_any_id(foreground, background) }
{	}

colour_pair::~colour_pair() {
	if(id!=-1) {
		release_idx(get_slot_idx());
	}
}


colour_pair::colour_pair(colour_pair&& other)
:	id { -1 } 
{
	swap(other);
}

colour_pair::colour_pair(const colour_pair& other) 
:	id { get_any_id(other.get_foreground(), other.get_background()) }
{	}

colour_pair& colour_pair::operator=(colour_pair other) {
	swap(other);
	return *this;
}


short colour_pair::get_pair_id() const {
	CONS_ASSERT(id > 0 && id <= COLOR_PAIRS, "colour pair id out of range");
	return id;
}
short colour_pair::get_slot_idx() const {
	short id=get_pair_id() - 1;
	CONS_ASSERT(id >= 0 && id <= short(slot_counts.size()),
		"colour pair id out of range");
	return id;
}

std::pair<short, short> colour_pair::get_pair() const {
	short f, b;
	pair_content(get_pair_id(), &f, &b);
	return std::make_pair(f, b);
}

short colour_pair::get_foreground() const {
	return get_pair().first;
}
short colour_pair::get_background() const {
	return get_pair().second;
}

void colour_pair::set_foreground(short foreground) {
	colour_pair tmp { foreground, get_background() };
	swap(tmp);
}
void colour_pair::set_background(short background) {
	colour_pair tmp { get_foreground(), background };
	swap(tmp);
}

short colour_pair::get_id() const {
	return COLOR_PAIR(get_pair_id());
}
bool colour_pair::operator==(const colour_pair& other) const {
	return other.get_pair() == get_pair(); //id == id should work too
}

void colour_pair::swap(colour_pair& other) {
	std::swap(id, other.id);
}

void swap(colour_pair& a, colour_pair& b) {
	a.swap(b);	
}

} //namespace cons

