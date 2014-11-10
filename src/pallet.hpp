#ifndef PALLET_HPP
#define PALLET_HPP

#include "ui/colour_pair.hpp"

#include <array>

enum class pallet_idx  {
	default_colour,

	title_bar,
	status_bar,

	notice_msg,

	join_msg,
	quit_msg,
	part_msg,
	title_msg,

	set_mode_msg,

	python_stdout,
	python_error,
	//! an ERROR: or ERR_X from the server
	irc_error,      
	//! we have recieved a msg but it's malformed or unrecognized (won't parse)
	protocol_error, 
	//! an issue with the connection it's self
	network_error,
	//! an error with the program itself
	system_error
};


class pallet {
	static constexpr std::size_t max_idx=
		static_cast<std::size_t>(pallet_idx::system_error) + 1;

	std::array<cons::colour_pair, max_idx> slots;

	std::size_t to_idx(pallet_idx i) const;
public:
	pallet();

	void set_foreground(pallet_idx pi, short c);
	void set_background(pallet_idx pi, short c);
	void set_colour_pair(pallet_idx pi, const cons::colour_pair& cp);
	short get_foreground(pallet_idx pi) const;
	short get_background(pallet_idx pi) const;
	const cons::colour_pair& get_colour_pair(pallet_idx pi) const;
	cons::colour_pair&       get_colour_pair(pallet_idx pi);
}; //class pallet

pallet& get_pallet();


short irc_to_cons_colour(irc::colours colour);
cons::colour_pair irc_to_pallet_colour(irc::colours fg, irc::colours bg);

#endif //PALLET_HPP
