/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:49:29 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/09 11:45:58 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExcept.hpp"

LimitExcept::LimitExcept(string context) {
    vector<string>      value_inline;
    vector<Directive *> value_block;

    if (context != "location")
        throw WrongContextExc("LimitExcept", "location", context);
    this->_type = "limit_except";
    this->_is_context = true;
    this->_value_inline = value_inline;
    this->_value_block = value_block;
}

LimitExcept::LimitExcept(string line, ifstream &block, string context) {
    if (context != "location")
        throw WrongContextExc("LimitExcept", "location", context);
    this->_type = "limit_except";
    this->_is_context = true;
    this->_parsing_inline(line);
    this->_parsing_block(block);
}

LimitExcept::~LimitExcept() {}
