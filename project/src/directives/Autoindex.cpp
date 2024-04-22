/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:40:16 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 20:43:46 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

Autoindex::Autoindex()
: ADirective("autoindex", GLOBAL_CONTEXT)
{}

Autoindex::Autoindex(uint16_t context, const std::string& content)
: ADirective("autoindex", context)
{
	std::stringstream	iss(content);
	std::string			token;
	unsigned int		tokenCount = 0;

	_isEnabled = false;
	while (iss >> token)
		++tokenCount;
	if (tokenCount == 1)
	{
		if (token == "on")
			_isEnabled = true;
		else if (token != "off")
			throw std::runtime_error("autoindex : invalid content");
	}
	else if (tokenCount > 1)
		throw std::runtime_error("autoindex : invalid content");

}

Autoindex::Autoindex(const Autoindex &copy)
: ADirective(copy)
{ *this = copy; }

Autoindex::~Autoindex() {}

Autoindex &Autoindex::operator=(const Autoindex &other)
{
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
