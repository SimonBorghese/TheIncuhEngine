/*
 * Logging.h
 *
 *	Holds several static functions for logging and such
 *
 *  Created on: Mar 3, 2022
 *      Author: simon
 */

#ifndef INCLUDE_INCUHENGINE_SERVER_LOGGING_H_
#define INCLUDE_INCUHENGINE_SERVER_LOGGING_H_
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>
#include <stdarg.h>
//#include <fmt/os.h>

void incuh_info(std::string message);
void incuh_log(std::string message);
void incuh_warning(std::string message);
void incuh_error(std::string message);


#endif /* INCLUDE_INCUHENGINE_SERVER_LOGGING_H_ */
