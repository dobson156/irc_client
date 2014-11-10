#include "log_messages.hpp"
#include "message.hpp"
#include "pallet.hpp"

#include <memory>
#include <string>

std::shared_ptr<text_message> make_python_error_msg(std::string msg) {
	return std::make_shared<text_message>(
		"PYTHON ERROR",
		get_pallet().get_colour_pair(pallet_idx::python_error),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::python_error)
	);
}


std::shared_ptr<text_message> make_python_stdout_msg(std::string msg) {
	return std::make_shared<text_message>(
		"PYTHON",
		get_pallet().get_colour_pair(pallet_idx::python_stdout),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::python_stdout)
	);
}


std::shared_ptr<text_message> make_network_error_msg(std::string msg) {
	return std::make_shared<text_message>(
		"NETWORK ERROR",
		get_pallet().get_colour_pair(pallet_idx::network_error),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::network_error)
	);
}

std::shared_ptr<text_message> make_system_error_msg(std::string msg) {
	return std::make_shared<text_message>(
		"SYSTEM ERROR",
		get_pallet().get_colour_pair(pallet_idx::system_error),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::system_error)
	);
}

std::shared_ptr<text_message> make_protocol_error_msg(std::string msg) {
	return std::make_shared<text_message>(
		"PROTOCOL ERROR",
		get_pallet().get_colour_pair(pallet_idx::protocol_error),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::protocol_error)
	);
}

std::shared_ptr<text_message> make_irc_error_msg(std::string msg) {
	return std::make_shared<text_message>(
		"IRC ERROR",
		get_pallet().get_colour_pair(pallet_idx::irc_error),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::irc_error)
	);
}

/*
TODO
std::shared_ptr<text_message> make_modes_msg() {

	std::ostringstream oss;
	oss << "Your mode has been changed to: " << md << " by " << set_by;

	return std::make_shared<text_message>(
		"Modes",
		get_pallet().get_colour_pair(pallet_idx::modes),
		std::move(msg),
		get_pallet().get_colour_pair(pallet_idx::modes)
	);
}
*/

