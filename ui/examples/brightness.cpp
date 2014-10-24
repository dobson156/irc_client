#include <console.hpp>
#include <progress_bar.hpp>

#include <fstream>
#include <string>

template<typename T>
T extract_val_from_file(const std::string& path) {
	std::ifstream btfile { path };
	T b;
	if(!(btfile >> b))
		throw std::runtime_error("unable to read from " + path);
	return b;
}
template<typename T>
void write_val_to_file(const std::string& path, const T& val) {
	std::ofstream btfile { path };
	if(!(btfile << val))
		throw std::runtime_error("unable to write to " + path);
}

const std::string brightness_dir="/sys/class/backlight/pwm-backlight.0";

int get_cur_brightness() {
	return extract_val_from_file<int>(brightness_dir + "/brightness");
}
int get_max_brightness() {
	return extract_val_from_file<int>(brightness_dir + "/max_brightness");
}
void set_cur_brightness(int val) {
//	std::cout << val << std::endl;
	write_val_to_file(brightness_dir + "/brightness", val);
}

int main() {
	try {
		cons::progress_bar pb { cons::make_window() };
		int max=get_max_brightness();
		pb.set_denominator(max);

		char c;
		do {
			int brightness=get_cur_brightness();
			pb.set_progress(brightness);
			pb.refresh();
			c=getch();

			if(c=='j') {
				set_cur_brightness(std::min(max, brightness + 100));
			}
			else if(c=='k') {
				set_cur_brightness(std::max(0, brightness - 100));
			}
		} while(c != 'q');
	}
	catch(const std::exception& e) {
		std::cerr << "An error ocrred: " << e.what() << std::endl;
	}
}
