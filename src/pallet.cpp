#include "pallet.hpp"

pallet::pallet() {
	slots[to_idx(pallet_idx::default_colour)]={ -1,            -1 };

//MESSAGE COLOURS
	slots[to_idx(pallet_idx::notice_msg)]    ={ COLOR_MAGENTA, -1 };
	slots[to_idx(pallet_idx::join_msg)]      ={ COLOR_GREEN,   -1 };
	slots[to_idx(pallet_idx::part_msg)]      ={ COLOR_CYAN,    -1 };

	slots[to_idx(pallet_idx::set_mode_msg)]  ={ COLOR_GREEN,   -1 };

//ERROR COLOURS
	slots[to_idx(pallet_idx::irc_error)]     ={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::protocol_error)]={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::network_error)] ={ COLOR_RED,     -1 };
	slots[to_idx(pallet_idx::system_error)]  ={ COLOR_RED,     -1 };
}

std::size_t pallet::to_idx(pallet_idx i) const {
	return static_cast<std::size_t>(i);
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
