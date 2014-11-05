#include "point.hpp"
#include "anchor_view.hpp"

#include <algorithm> //std::min/max

namespace cons { 
namespace anchors {

top::top(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point top::get_fill_position() const {
	auto used=get_anchor_dimension();
	return { 0, used.y };
}
point top::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x, dimension.y - used.y };
}
point top::get_anchor_position() const {
	return { 0, 0 };
}
point top::get_anchor_dimension() const {
	return { dimension.x, std::min(n, dimension.y) };
}
void top::set_dimension(point dimension_) { dimension=dimension_; }
void top::set_n(int n_) { n=n_; }
int top::get_n() const { return n; }

left::left(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point left::get_fill_position() const {
	auto used=get_anchor_dimension();
	return { used.x, 0 };
}
point left::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x - used.x, dimension.y };
}
point left::get_anchor_position() const {
	return { 0, 0 };
}
point left::get_anchor_dimension() const {
	return { std::min(n, dimension.x), dimension.y };
}
void left::set_dimension(point dimension_) { dimension=dimension_; }
void left::set_n(int n_) { n=n_; }
int left::get_n() const { return n; }

bottom::bottom(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point bottom::get_fill_position() const {
	return { 0, 0 };
}
point bottom::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x, dimension.y - used.y };
}
point bottom::get_anchor_position() const {
	return { 0, std::max(dimension.y-n, 0)  };
}
point bottom::get_anchor_dimension() const {
	return { dimension.x, std::min(n, dimension.y) };
}
void bottom::set_dimension(point dimension_) { dimension=dimension_; }
void bottom::set_n(int n_) { n=n_; }
int bottom::get_n() const { return n; }

right::right(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point right::get_fill_position() const {
	return { 0, 0 };
}
point right::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x - used.x, dimension.y };
}
point right::get_anchor_position() const {
	return { std::max(dimension.x-n, 0), 0 };
}
point right::get_anchor_dimension() const {
	return { std::min(n, dimension.x), dimension.y };
}
void right::set_dimension(point dimension_) { dimension=dimension_; }
void right::set_n(int n_) { n=n_; }
int right::get_n() const { return n; }

} //namespace anchors
} //namespace cons
