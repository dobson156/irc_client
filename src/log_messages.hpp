#ifndef LOG_MESSAGES_HPP
#define LOG_MESSAGES_HPP

#include "message.hpp"

#include <string>
#include <memory>

std::shared_ptr<text_message> make_python_error_msg(std::string msg);
std::shared_ptr<text_message> make_network_error_msg(std::string msg);
std::shared_ptr<text_message> make_network_error_msg(std::string msg);
std::shared_ptr<text_message> make_protocol_error_msg(std::string msg);
std::shared_ptr<text_message> make_irc_error_msg(std::string msg);

#endif //LOG_MESSAGES_HPP
