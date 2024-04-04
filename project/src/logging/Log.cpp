/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:17:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/04 15:20:59 by kichkiro         ###   ########.fr       */
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

void Log::info(const string &msg) {
	cout << BLUE << _timeStamp() << " [INFO]\t" << WHITE << msg << endl;
}

void Log::error(const string &msg) {
	cout << RED << _timeStamp() << " [ERROR]\t" << WHITE << msg << endl;
}

void Log::response(int method, std::string httpVersion, std::string uri, int status) {
	switch (status) {
		case 200:
			cout << GREEN;
			break;
		case 400:
			cout << YELLOW;
			break;
		default:
			cout << RED;
			break;
	}

	cout << _timeStamp();

	switch (method) {
		case GET:
			cout << " [GET]\t";
			break;
		case POST:
			cout << " [POST]\t";
			break;
		case DELETE:
			cout << " [DELETE]\t";
			break;
		default:
			cout << " [UNKNOWN]\t";
			break;
	}

	cout << WHITE << status << " " << httpVersion << " " << uri << endl;
}
