/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:17:08 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/15 14:46:06 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Colors --------------------------------------------------------------------->

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;5;208m"
#define BLUE "\033[34m"
#define WHITE "\033[37m"

// Class ---------------------------------------------------------------------->

class Log {
	private:
		Log(void);
		Log(const Log &src);
		Log &operator=(const Log &src);
		~Log(void);

		static std::string _timeStamp(void);
	public:
		static void debug(const std::string &msg);
		static void info(const std::string &msg);
		static void error(const std::string &msg);
		static void response(int method, std::string httpVersion, std::string uri, int status);
};
