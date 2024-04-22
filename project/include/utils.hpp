/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 10:42:41 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:38:00 by kichkiro         ###   ########.fr       */
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

bool				isComment(const std::string &s);
bool				isBracket(const std::string &s);
bool				isClosedBracket(const std::string &s);
bool				isServerDirective(const std::string &s);
bool				checkContext(int directiveContext, uint16_t currentContext);
int					whichDirective(const std::string &s);
std::string			strip(const std::string &str);
std::string			firstToken(const std::string &str);
std::string			secondToken(const std::string &str);
std::vector<uint16_t>	getContextValues(int directive);
