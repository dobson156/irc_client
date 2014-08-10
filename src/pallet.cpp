#include <irc/parse_coloured_string.hpp>

#include "pallet.hpp"

#include <cassert>

pallet::pallet() {
	slots[to_idx(pallet_idx::default_colour)]={ -1,            -1 };

//MESSAGE COLOURS
	slots[to_idx(pallet_idx::notice_msg)]    ={ COLOR_MAGENTA, -1 };
	slots[to_idx(pallet_idx::join_msg)]      ={ COLOR_YELLOW,  -1 };
	slots[to_idx(pallet_idx::part_msg)]      ={ COLOR_BLUE,    -1 };

	slots[to_idx(pallet_idx::set_mode_msg)]  ={ COLOR_GREEN,   -1 };

//ERROR COLOURS
	slots[to_idx(pallet_idx::python_error)]  ={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::irc_error)]     ={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::protocol_error)]={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::network_error)] ={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::system_error)]  ={ COLOR_RED,     -1 };
}

std::size_t pallet::to_idx(pallet_idx i) const {
	std::size_t n=static_cast<std::size_t>(i);
	assert(n < slots.size());
	return n;
}

void pallet::set_foreground(pallet_idx pi, short c) {
	slots[to_idx(pi)].set_foreground(c);
}
void pallet::set_background(pallet_idx pi, short c) {
	slots[to_idx(pi)].set_background(c);
}
void pallet::set_colour_pair(pallet_idx pi, const cons::colour_pair& cp) {
	slots[to_idx(pi)]=cp;
}
short pallet::get_foreground(pallet_idx pi) const {
	return slots[to_idx(pi)].get_foreground();
}
short pallet::get_background(pallet_idx pi) const {
	return slots[to_idx(pi)].get_background();
}
const cons::colour_pair& pallet::get_colour_pair(pallet_idx pi) const {
	return slots[to_idx(pi)];
}
cons::colour_pair& pallet::get_colour_pair(pallet_idx pi) {
	return slots[to_idx(pi)];
}

pallet& get_pallet() {
	static pallet pal;
	return pal;
}

short irc_to_cons_colour(irc::colours colour) {
	switch(colour) {
	case irc::colours::white: return COLOR_WHITE;
	case irc::colours::black: return COLOR_BLACK;
	case irc::colours::blue: return COLOR_BLUE;
	case irc::colours::green: return COLOR_GREEN;
	case irc::colours::red: return COLOR_RED;
	case irc::colours::brown: return COLOR_RED; //bad
	case irc::colours::purple: return COLOR_BLUE; //bad
	case irc::colours::orange: return COLOR_YELLOW; //bad
	case irc::colours::yellow: return COLOR_YELLOW;
	case irc::colours::light_green: return COLOR_GREEN; //bad
	case irc::colours::teal: return COLOR_CYAN; //bad
	case irc::colours::light_cyan: return COLOR_CYAN;
	case irc::colours::light_blue: return COLOR_BLUE; //bad
	case irc::colours::pink: return COLOR_RED; //bad
	case irc::colours::grey: return COLOR_BLACK; //bad
	case irc::colours::light_grey: return COLOR_WHITE; //bad
	default: return -1;
	}
} //enum class colours

cons::colour_pair irc_to_pallet_colour(irc::colours fg, irc::colours bg) {
	return { irc_to_cons_colour(fg), irc_to_cons_colour(bg) };
}
