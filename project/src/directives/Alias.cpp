/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Alias.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:39:58 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:40:48 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Alias.hpp"

Alias::Alias() : ADirective("Alias", GLOBAL_CONTEXT) {}

Alias::Alias(uint16_t context, const std::string &path)
	: ADirective("Alias", context), _path(path) {}

Alias::Alias(const Alias &copy) : ADirective(copy) {
	*this = copy;
}

Alias::~Alias() {}

Alias &Alias::operator=(const Alias &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_path = other._path;

	return *this;
}

ADirective *Alias::clone() const {
	return new Alias(*this);
}

const std::string &Alias::getPath() const {
	return _path;
}
