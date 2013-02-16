#ifndef ANCHOR_VIEW_HPP
#define ANCHOR_VIEW_HPP

#include "util.hpp"

#include "basic.hpp"
#include "assert.hpp"

#include <exception>
#include <utility>

namespace cons {

template<typename AnchorType>
class anchor_view : public base {
public:
	using anchor_type=AnchorType;
private:
	frame           frame_;
	anchor_type     anchor_control;
	unique_base_ptr anchor;
	unique_base_ptr fill;

	void shuffle() {
		if(anchor) {
			try {
				anchor->set_position(anchor_control.get_anchor_position());
				anchor->set_dimension(anchor_control.get_anchor_dimension());
			}
			catch(const std::exception&) {
				anchor->set_dimension(anchor_control.get_anchor_dimension());
				anchor->set_position(anchor_control.get_anchor_position());
			}
		}
		if(fill) {
			try {
				fill->set_position(anchor_control.get_fill_position());
				fill->set_dimension(anchor_control.get_fill_dimension());
			}
			catch(const std::exception&) {
				fill->set_dimension(anchor_control.get_fill_dimension());
				fill->set_position(anchor_control.get_fill_position());
			}
		}
		touchwin(frame_.get_handle());
	}
public:
	anchor_view(unique_window_ptr handle_, int partition) 
	:	frame_         ( std::move(handle_) )
	,	anchor_control { frame_.get_dimension(), partition   }
	{	}

	template<typename ElementType, typename... Args>
	ElementType& emplace_fill(Args&&...additional_args) {
		auto element_fill=util::make_unique<ElementType>(
			make_window(
				frame_.get_handle(),
				anchor_control.get_fill_position(),	
				anchor_control.get_fill_dimension()
			),
			std::forward<Args>(additional_args)...
		);
		auto raw_ptr=element_fill.get();
		fill=std::move(element_fill);
		return *raw_ptr;
	}
	template<typename ElementType, typename... Args>
	ElementType& emplace_anchor(Args&&...additional_args) {
		auto element_anchor=util::make_unique<ElementType>(
			make_window(
				frame_.get_handle(),
				anchor_control.get_anchor_position(),	
				anchor_control.get_anchor_dimension()
			),
			std::forward<Args>(additional_args)...
		);
		auto raw_ptr=element_anchor.get();
		anchor=std::move(element_anchor);
		return *raw_ptr;
	}	
	template<typename ElementType=base>
	ElementType& get_fill() {
		CONS_ASSERT(fill!=nullptr, "fill pointer is null");
	#ifdef NDEBUG 
		return static_cast<ElementType&>(*fill);
	#else
		auto t=dynamic_cast<ElementType*>(fill.get());
		CONS_ASSERT(t!=nullptr, 
			"requested type does not meet type requirements");
		return *t;
	#endif
	}
	template<typename ElementType=base>
	ElementType& get_anchor() {
		CONS_ASSERT(anchor!=nullptr, "anchor pointer is null");
	#if NDEBUG 
		return static_cast<ElementType&>(*anchor);
	#else
		auto t=dynamic_cast<ElementType*>(anchor.get());
		CON_ASSERT(t!=nullptr,
			"requested type does not meet type requirements");
		return *t;
	#endif
	}
//Overrides
	void clear() override {
		if(fill)   fill->clear();
		if(anchor) anchor->clear();
		frame_.clear();
	}
	void refresh() override {
		if(fill)   fill->refresh();
		if(anchor) anchor->refresh();
		frame_.refresh();
	}
	void set_position(const point& position) override {
		frame_.set_position(position);
		shuffle();
	}
	void set_dimension(const point& dimension) override {
		frame_.set_dimension(dimension);
		anchor_control.set_dimension(dimension);
		shuffle();
	}
	point get_dimension() const override { return frame_.get_dimension(); }
	point get_position()  const override { return frame_.get_position();  }
	int   get_partition() const          { return anchor_control.get_n(); }

	void set_partition(int n) {
		anchor_control.set_n(n);		
		shuffle();
	}
}; //class anchor_view

namespace anchors {

#define ANCHOR_PROTOTYPE(name) \
class name { \
	point    dimension;\
    int      n; \
public: \
	name(point dimension_, int n_); \
    point get_fill_position() const; \
    point get_fill_dimension() const; \
    point get_anchor_position() const; \
    point get_anchor_dimension() const; \
	void set_dimension(point dimension_); \
	void set_n(int n); \
	int get_n() const; \
};

ANCHOR_PROTOTYPE(top)
ANCHOR_PROTOTYPE(left)
ANCHOR_PROTOTYPE(right)
ANCHOR_PROTOTYPE(bottom)

#undef ANCHOR_PROTOTYPE
} //namespace anchors

} //nameapce cons

#endif //ANCHOR_VIEW_HPP
