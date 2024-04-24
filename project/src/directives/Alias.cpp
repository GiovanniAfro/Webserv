/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Alias.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:39:58 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 21:45:21 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Alias.hpp"

Alias::Alias()
: ADirective("alias", GLOBAL_CONTEXT)
{}

Alias::Alias(const std::string &path)
: ADirective("alias", LOCATION_CONTEXT), _path(path)
{}

Alias::Alias(const Alias &copy) : ADirective(copy)
{ *this = copy; }

Alias::~Alias() {}

Alias &Alias::operator=(const Alias &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_path = other._path;

	return *this;
}

ADirective *Alias::clone() const
{ return new Alias(*this); }

const std::string &Alias::getPath() const
{ return _path; }
