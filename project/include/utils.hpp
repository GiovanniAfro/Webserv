/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 10:42:41 by kichkiro          #+#    #+#             */
/*   Updated: 2024/05/04 23:16:20 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Libraries ------------------------------------------------------------------>

#include <iostream>
// #include <stdint.h>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>

// using std::string;
// using std::vector;

// Functions ------------------------------------------------------------------>

bool					isComment(const std::string &s);
bool					isBracket(const std::string &s);
bool					isClosedBracket(const std::string &s);
bool					checkContext(int directiveContext, uint16_t currentContext);
bool					isLocationModifier(const std::string& s);
bool					isIPAddress(const std::string& s);
int						whichDirective(const std::string &s);
std::string				strip(const std::string &str);
std::string				firstToken(const std::string &str);
std::string				secondToken(const std::string &str);
std::vector<uint16_t>	getContextValues(int directive);

bool					isFolder(const std::string &path);
bool					isFile(const std::string &path);
std::string				strRemove(std::string const &str, std::string const &toRemove);
std::string				strReplace(std::string const &str, std::string const &from, std::string const &to);
void                    split_path(const std::string &full_path, std::string &directory, std::string &filename);
