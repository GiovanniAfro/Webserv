/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:17:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/20 23:29:14 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

std::string Log::_timeStamp() {
	time_t t = time(0);
	tm *now = localtime(&t);
	std::stringstream ss;
	ss << "[" << (now->tm_year + 1900)
		<< "-" << std::setw(2) << std::setfill('0') << (now->tm_mon + 1)
		<< "-" << std::setw(2) << std::setfill('0') << now->tm_mday
		<< " " << std::setw(2) << std::setfill('0') << now->tm_hour
		<< ":" << std::setw(2) << std::setfill('0') << now->tm_min
		<< ":" << std::setw(2) << std::setfill('0') << now->tm_sec << "]";
	return (ss.str());
}

void Log::debug(const std::string &msg)
{
	std::cout << ORANGE << _timeStamp() << " [DEBUG]\t" << WHITE << msg << std::endl;
}

void Log::info(const std::string &msg)
{
	std::cout << BLUE << _timeStamp() << " [INFO]\t" << WHITE << msg << std::endl;
}

int Log::error(const std::string &msg) {
	std::cout << RED << _timeStamp() << " [ERROR]\t" << WHITE << msg << std::endl;
	return -1;
}

void Log::response(int method, std::string httpVersion, std::string uri, int status) {
	switch (status) {
		case 200:
			std::cout << GREEN;
			break;
		case 400:
			std::cout << YELLOW;
			break;
		default:
			std::cout << RED;
			break;
	}

	std::cout << _timeStamp();

	switch (method) {
		case GET:
			std::cout << " [GET]\t";
			break;
		case POST:
			std::cout << " [POST]\t";
			break;
		case DELETE:
			std::cout << " [DELETE]\t";
			break;
		default:
			std::cout << " [UNKNOWN]\t";
			break;
	}

	std::cout << WHITE << status << " " << httpVersion << " " << uri << std::endl;
}
