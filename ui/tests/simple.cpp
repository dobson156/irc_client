#include <console.hpp>

int main() {

	cons::frame f { cons::make_window() };

	cons::frame g { cons::make_window(f.get_handle(), {10, 10}, {10,10}) };

	g.write({0,0},"hi");
	g.refresh();

	wgetch(g.get_handle());

	g.set_dimension({10,11});
	g.set_position({2,2});
	
	g.clear();
	g.write({0,0},"bi");
	g.refresh();
	f.refresh();

	wgetch(g.get_handle());
}
