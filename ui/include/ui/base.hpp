#ifndef CONS_BASE_HPP
#define CONS_BASE_HPP

#include "point.hpp"
#include "window.hpp"

#include <memory>

namespace cons {

class base {
/**
 * This is the abstract interface that all UI elements must provide
 */
public:
	/**
	 * virtual dtor, UI elements may well be own by items that only know 
	 * they decend from base
	 */
	virtual ~base() { }
	/**
     * This function resets the state of the UI element back to it's blank state,
	 * for example, a text input would become empty, a frame would be wiped completely
	 * this doesn't mean that nothing would be deplayed
	 */
	virtual void clear()                              =0;
	/**
	 * This function refreshes the UI element, this step may cause the
	 * UI element to redraw and refresh (push output to screen)
	 */
	virtual void refresh()                            =0;
	/**
	 * sets the position of this element to a new location inside of it's 
	 * parent window
	 *
	 * @param position the new position as a point
	 */
	virtual void set_position(const point& position)  =0;
	/**
	 * sets the dimension of this element
	 *
	 * @param dimension the new dimension as a point
	 */
	virtual void set_dimension(const point& dimension)=0;
	/**
	 * @return the current position as a point
	 */
	virtual point get_position() const                =0;
	/**
	 * @return the current dimension as a point
	 */
	virtual point get_dimension() const               =0;

	/**
	 * resets the underlying resource handle
	 * this is useful when we wish to rebuild the screen (perhaps due to
	 * resizing of the console or if we want to change the dimension and
	 * size of a UI element (giving it a handle to a resource with the desired
	 * proportions
	 *
	 * @param a unique_ptr to the new resource, this UI element will now manage
	 * the lifetime of this resource and the old resource will disposed of
	 */
	virtual unique_window_ptr reset(unique_window_ptr)=0;
}; //class base

using unique_base_ptr  =std::unique_ptr<base>;

} //namespace cons

#endif //CONS_BASE_HPP
