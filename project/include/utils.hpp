/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 10:42:41 by kichkiro          #+#    #+#             */
/*   Updated: 2024/03/12 13:56:05 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Libraries ------------------------------------------------------------------>

#include <iostream>
#include <stdint.h>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>

using std::string;
using std::vector;

// Functions ------------------------------------------------------------------>

bool uint16_t_in_vec(const vector<uint16_t> &vec, int target);
bool str_in_array(const char *str, const char **arr);
string strip(const string &str);
string first_token(const string &str);
string second_token(const string &str);
