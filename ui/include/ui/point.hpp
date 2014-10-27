#ifndef CONS_POINT_HPP 
#define CONS_POINT_HPP

#include <ostream>

namespace cons {

struct point { 
/**
 * A co-ordinate or offset
 * when constructued using brace syntax, ie: { 5, 6 } then the first elemnt 
 * is x, and the second is y
 * 
 * for sizes (ie: get_dimension()) then the values are the number of positions, 
 * so a square element with a single block is of size 1,1
 *
 * positions are generally indexes into those positions, 
 * so the first position is {0,0}, and the last is dimension - {1,1}
 */
	int x, y; 
	point()                       =default;
	point(const point&)           =default;
	point& operator=(const point&)=default;
}; //struct point

std::ostream& operator<<(std::ostream& os, const point& p);
bool  operator==(const point& l, const point& r);
bool  operator< (const point& l, const point& r);
/**
 * returns { x1 + x2, y1 + y2 }
 * @return the new value
 */
point operator+ (const point& l, const point& r);
/**
 * returns { x1 - x2, y1 - y2 }
 * @return the new value
 */
point operator- (const point& l, const point& r);

} //namespace cons

#endif // CONS_POINT_HPP
