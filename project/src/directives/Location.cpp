/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:49:29 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/09 11:46:19 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(string context) {
    vector<string>      value_inline;
    vector<Directive *> value_block;

    if (context != "server" && context != "location")
        throw WrongContextExc("Location", "server|location", context);
    this->_type = "location";
    this->_is_context = true;
    this->_value_inline = value_inline;
    this->_value_block = value_block;
}

Location::Location(string line, ifstream &block, string context) {
    if (context != "server" and context != "location")
        throw WrongContextExc("Location", "server|location", context);
    this->_type = "location";
    this->_is_context = true;
    this->_parsing_inline(line);
    this->_parsing_block(block);
}

Location::~Location() {}
