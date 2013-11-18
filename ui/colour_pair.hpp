#ifndef COLOUR_PAIR_HPP
#define COLOUR_PAIR_HPP

#include <vector>
#include <utility>

namespace cons {


class colour_pair {
private:
	// the following vector identies how many colours pairs map to the ncurses
	// colour pair with the id == the index in the vector+1 (ids start from 1)
	static std::vector<unsigned> slot_counts;	
	//Shorts are used as that is what is used by ncurses, see init_pair
	static short find_slot_with_colour(short fg, short bg);
	static short get_first_free_slot();
	//returns the first matching slot or aquires a new one
	static short get_any_id(short fg, short bg);
	static void release_idx(short id); 

	short id { 0 };
	std::pair<short, short> get_pair() const;

	static void init_pair_with_id(short id, short fg, short bg);
	
	//returns the id, asserts
	short get_pair_id() const;
	//returns the idx for the slot associated with the colour pair
	short get_slot_idx() const;
public:
//CTOR
	colour_pair();
	colour_pair(short foreground, short background);
	colour_pair(colour_pair&& other);
	colour_pair(const colour_pair& other);
	~colour_pair();
//OPS
	colour_pair& operator=(colour_pair pair);
	bool operator==(const colour_pair& other) const;
//Mutator
	void set_foreground(short foreground);
	void set_background(short background);
	void swap(colour_pair& other);
//Accessor
	short get_foreground() const;
	short get_background() const;
	short get_id() const;
}; //class colour_pair

void swap(colour_pair& a, colour_pair& b);

} //namespace cons

#endif // COLOUR_PAIR_HPP
