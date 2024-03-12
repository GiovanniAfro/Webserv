/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:17:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/12 16:35:39 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

string Log::_timeStamp() {
	time_t t = time(0);
	tm *now = localtime(&t);
	stringstream ss;
	ss << "[" << (now->tm_year + 1900)
		<< "-" << setw(2) << setfill('0') << (now->tm_mon + 1)
		<< "-" << setw(2) << setfill('0') << now->tm_mday
		<< " " << setw(2) << setfill('0') << now->tm_hour
		<< ":" << setw(2) << setfill('0') << now->tm_min
		<< ":" << setw(2) << setfill('0') << now->tm_sec << "]";
	return (ss.str());
}

void Log::debug(const string &msg) {
	cout << WHITE << _timeStamp() << " [DEBUG]\t" << msg << endl;
}

void Log::info(const string &msg) {
	cout << BLUE << _timeStamp() << " [INFO]\t" << WHITE << msg << endl;
}

void Log::warning(const string &msg) {
	cout << YELLOW << _timeStamp() << " [WARNING]\t" << WHITE << msg << endl;
}

void Log::error(const string &msg) {
	cerr << RED << _timeStamp() << " [ERROR]\t" << WHITE << msg << endl;
}

void Log::request(const string &request) {
	cout << GREEN << _timeStamp() << " [REQUEST]\t" << WHITE << request << endl;
}
