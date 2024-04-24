/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:45:50 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 15:07:57 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

std::vector<LOCATION_MODIFIER>	allLocationModifiers;

void initializeLocationVector()
{
	if (!allLocationModifiers.empty())
		return;
	allLocationModifiers.push_back(NONE);
	allLocationModifiers.push_back(EXACT);
	allLocationModifiers.push_back(CASE_SENSITIVE_REGEX);
	allLocationModifiers.push_back(CASE_INSENSITIVE_REGEX);
	allLocationModifiers.push_back(LONGEST_NON_REGEX);
}


Location::Location(uint16_t context, enum LOCATION_MODIFIER modifier, const std::string& uri)
: ADirective("location", context)
{
	initializeLocationVector();
	bool	isModifier = false;

	for (std::vector<LOCATION_MODIFIER>::iterator it = allLocationModifiers.begin(); it != allLocationModifiers.end(); ++it)
		if (modifier == *it)
			isModifier = true;
	if (!isModifier)
		throw std::runtime_error("location : invalid modifier");

	_modifier = modifier;
	_uri = uri;
}

Location::Location(const Location &copy) : ADirective(copy)
{ *this = copy; }

Location::~Location() {}

Location &Location::operator=(const Location &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_modifier = other._modifier;
	this->_uri = other._uri;

	return *this;
}

ADirective*	Location::clone() const
{ return new Location(*this); }

enum LOCATION_MODIFIER	Location::getModifier() const
{ return _modifier; }

const std::string&	Location::getUri() const
{ return _uri; }


enum LOCATION_MODIFIER	Location::_modifierToEnum(const std::string& modifier)
{
	if (modifier.empty())
		return NONE;
	else if (modifier == "=")
		return EXACT;
	else if (modifier == "~")
		return CASE_SENSITIVE_REGEX;
	else if (modifier == "~*")
		return CASE_INSENSITIVE_REGEX;
	else if (modifier == "^~")
		return LONGEST_NON_REGEX;

	return INVALID;
}

std::string	Location::_modifierToString(enum LOCATION_MODIFIER modifier)
{
	switch (modifier)
	{
		case NONE:
			return "";
		case EXACT:
			return "=";
		case CASE_SENSITIVE_REGEX:
			return "~";
		case CASE_INSENSITIVE_REGEX:
			return "~*";
		case LONGEST_NON_REGEX:
			return "^~";
		case INVALID:
			return "";
		default:
			return "";
	}
}
