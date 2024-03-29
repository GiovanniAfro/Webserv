/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 09:51:48 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/29 14:57:33 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Libraries ------------------------------------------------------------------>

#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include <iterator>

#include <arpa/inet.h>
#include <poll.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

#include "utils.hpp"

// Using ---------------------------------------------------------------------->

using std::string;
using std::stringstream;
using std::cerr;
using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::time_t;
using std::time;
using std::tm;
using std::localtime;
using std::ifstream;
using std::vector;
using std::getline;
using std::streampos;
using std::exception;
using std::remove;
using std::ofstream;
using std::runtime_error;

// Classes -------------------------------------------------------------------->

#include "Directive.hpp"
#include "Log.hpp"
