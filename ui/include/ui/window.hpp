#ifndef CONS_WINDOW_HPP
#define CONS_WINDOW_HPP

#include <ncurses.h>

#include <memory>

namespace cons {

struct point;

using unique_window_ptr=std::unique_ptr<WINDOW, decltype(&::delwin)>;
//helper
int destroy_parent(WINDOW *win);
//Makes a new parent windows
unique_window_ptr make_window();
//Makes a sub window from an existing window
unique_window_ptr make_window(WINDOW *parent, const point& position, 
                                              const point& dimension);
//Makes a new pad
unique_window_ptr make_pad(const point& dimension);

} //namespace cons

#endif //CONS_WINDOW_HPP
