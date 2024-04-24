/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:44:35 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 11:27:01 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExcept.hpp"

LimitExcept::LimitExcept()
: ADirective("limit_except", GLOBAL_CONTEXT)
{}

LimitExcept::LimitExcept(uint16_t context, enum HTTP_METHOD method)
: ADirective("limit_except", context), _method(method)
{
	bool	methodOk = false;

	for (std::vector<HTTP_METHOD>::iterator itMethod = allHttpMethods.begin();itMethod != allHttpMethods.end(); ++itMethod)
		if (*itMethod == _method)
			methodOk = true;
	if (!methodOk)
		throw std::runtime_error("limit_except : invalid method");
}

LimitExcept::LimitExcept(const LimitExcept &copy) : ADirective(copy)
{ *this = copy; }

LimitExcept::~LimitExcept() {}

LimitExcept &LimitExcept::operator=(const LimitExcept &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_method = other._method;

	return *this;
}

ADirective *LimitExcept::clone() const
{ return new LimitExcept(*this); }

enum HTTP_METHOD	LimitExcept::getMethod() const
{ return _method; }
