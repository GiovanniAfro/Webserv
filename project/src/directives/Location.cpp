/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:45:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:46:24 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : ADirective("Location", GLOBAL_CONTEXT) {}

Location::Location(uint16_t context) : ADirective("Location", context) {}

Location::Location(const Location &copy) : ADirective(copy) {
	*this = copy;
}

Location::~Location() {}

Location &Location::operator=(const Location &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);


	return *this;
}

ADirective *Location::clone() const {
	return new Location(*this);
}


