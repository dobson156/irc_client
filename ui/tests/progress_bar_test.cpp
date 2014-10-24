#include <console.hpp>
#include <progress_bar.hpp>

int main() {
	cons::frame f { cons::make_window() };

	cons::progress_bar pb { cons::make_window(f.get_handle(), {10, 10}, {10,10}) };
	pb.set_denominator(8);

	for(int i=0; i!=9; ++i) {
		pb.set_progress(i);
		pb.refresh();
		getch();
	}
}
