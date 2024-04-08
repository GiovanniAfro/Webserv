/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:48:03 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/08 16:18:56 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

Listen::Listen(string context) {
    vector<string> value;

    if (context != "server")
        throw WrongContextExc("Listen", "server", context);
    value.push_back("*:80");
    this->_type = "listen";
    this->_is_context = false;
    this->_value_inline = value;
}

Listen::Listen(string raw_value, string context) {
    if (context != "server")
        throw WrongContextExc("Listen", "server", context);
    this->_type = "listen";
    this->_is_context = false;
    this->_parsing_inline(raw_value);
    this->_validate_config();
}

Listen::~Listen() {}

void Listen::_validate_config(void) {
    if (this->_value_inline.size() != 1 and this->_value_inline.size() != 2)
        throw WrongNumArgsExc(this->_type);
    else if (this->_value_inline.size() == 2 and 
             this->_value_inline[1] != "default_server")
        throw WrongValueArgExc(this->_type, this->_value_inline[1]);
}
