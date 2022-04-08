/*
 * Logging.cpp
 *
 *  Created on: Mar 3, 2022
 *      Author: simon
 */

#include <Server/Logging.h>

void incuh_info(std::string message){
	fmt::print(fmt::fg(fmt::color::blue), message);
}
void incuh_log(std::string message){
	fmt::print(fmt::fg(fmt::color::dark_blue), message);
}
void incuh_warning(std::string message){
	fmt::print(fmt::fg(fmt::color::orange), message);

}
void incuh_error(std::string message){
	fmt::print(fmt::fg(fmt::color::red), message);
}
