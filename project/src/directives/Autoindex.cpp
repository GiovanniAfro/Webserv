/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:40:16 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:41:02 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

Autoindex::Autoindex() : ADirective("Autoindex", GLOBAL_CONTEXT) {}

Autoindex::Autoindex(uint16_t context, bool isEnabled)
	: ADirective("Autoindex", context), _isEnabled(isEnabled) {}

Autoindex::Autoindex(const Autoindex &copy) : ADirective(copy) {
	*this = copy;
}

Autoindex::~Autoindex() {}

Autoindex &Autoindex::operator=(const Autoindex &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_isEnabled = other._isEnabled;

	return *this;
}

ADirective *Autoindex::clone() const {
	return new Autoindex(*this);
}

bool	Autoindex::getMode() const {
	return _isEnabled;
}
