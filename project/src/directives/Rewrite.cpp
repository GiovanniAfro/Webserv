/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rewrite.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:46:49 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/25 17:32:19 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Rewrite.hpp"

Rewrite::Rewrite()
: ADirective("rewrite", GLOBAL_CONTEXT)
{}

Rewrite::Rewrite(uint16_t context, const std::string& uri, const std::string& replacement, bool redirect)
: ADirective("rewrite", context), _uri(uri), _replacement(replacement), _redirect(redirect)
{
	if (_uri.empty() || _replacement.empty())
		throw std::runtime_error("rewrite : invalid content");
}

Rewrite::Rewrite(const Rewrite &copy)
: ADirective(copy)
{ *this = copy; }

Rewrite::~Rewrite() {}

Rewrite&	Rewrite::operator=(const Rewrite &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_uri = other._uri;
	this->_replacement = other._replacement;
	this->_redirect = other._redirect;

	return *this;
}

ADirective*	Rewrite::clone() const
{ return new Rewrite(*this); }


const std::string&	Rewrite::getUri() const
{ return _uri; }

const std::string&	Rewrite::getReplacement() const
{ return _replacement; }

bool	Rewrite::isRedirect() const
{return _redirect; }
