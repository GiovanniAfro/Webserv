/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 09:51:48 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/26 13:39:14 by kichkiro         ###   ########.fr       */
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
#include <algorithm>
#include <cctype>
#include <set>
#include <deque>
#include <limits>

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
#include <glob.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>

#include "utils.hpp"

// Classes -------------------------------------------------------------------->

#include "ADirective.hpp"
#include "Http.hpp"
#include "Include.hpp"
#include "Server.hpp"
#include "Listen.hpp"
#include "Root.hpp"
#include "ServerName.hpp"
#include "Alias.hpp"
#include "Autoindex.hpp"
#include "ClientMaxBodySize.hpp"
#include "ErrorPage.hpp"
#include "LimitExcept.hpp"
#include "Location.hpp"
#include "Rewrite.hpp"
#include "Index.hpp"
#include "Cgi.hpp"
#include "Log.hpp"
#include "WebServer.hpp"
#include "ConfigFile.hpp"
#include "Socket.hpp"

// Macros ---------------------------------------------------------------------->


// #define GLOBAL_CONTEXT 0
// #define HTTP_CONTEXT 1
// #define SERVER_CONTEXT 2
// #define LOCATION_CONTEXT 3

// #define NUM_DIRECTIVES 7
// #define HTTP_DIRECTIVE 0
// #define INCLUDE_DIRECTIVE 1
// #define SERVER_DIRECTIVE 2
// #define LISTEN_DIRECTIVE 3
// #define ROOT_DIRECTIVE 4
// #define SERVER_NAME_DIRECTIVE 5
// #define INDEX_DIRECTIVE 6
