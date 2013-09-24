#include "colour_pair.hpp"

namespace cons {

static short colour_pair::find_slot_with_colour(short fg, short bg) {
	short ft, bt;
	for(short i=1; i<=slots_count.size(); ++i) {
		pair_content(i, &ft, &bt);
		if(ft==fg && bg==bt) return i;
	}
	return -1; //colour not found
}

static short colour_pair::get_first_free_slot() {
	auto it=std::find(begin(slots_count), end(slots_count), 0)
	if(it!=end(slots_count) 
		return std::distance(begin(slots_count), it)+1;

	if(slots_count.size() >= COLOR_PAIRS) {
		std::ostringstream oss;
		oss << "failed to aquire new slot, number of existing slots: " 
		    << slots_count.size() << " maximum number of slots "
			<< COLOR_PAIRS;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	slot_count.push_back(0);

	return slots.size();
}

static short colour_pair::get_any_slot(short fg, short bg) {
	short slot=find_slot_with_colours(fg, bg);
	if(slot==-1) { //the pair does not currently have a slot
		slot=get_first_free_slot();
	}
	++slot_counts[slot];
	return slot;
}

static void release_id(short id) {
	--id;
	CONS_ASSERT(id >= 0 && id < slot_counts.size(), 
		"can not release slot at invalid idx");
	--slot_counts[id];
}

} //namespace cons

