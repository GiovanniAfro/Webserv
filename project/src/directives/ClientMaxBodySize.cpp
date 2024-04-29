/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:40:36 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/29 20:11:10 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize(uint16_t context, unsigned long long size)
: ADirective("client_max_body_size", context), _size(size)
{
	if (size > CLIENT_MAX_BODY_SIZE)
		throw std::runtime_error("client_max_body_size : size too large");
}

ClientMaxBodySize::ClientMaxBodySize(const ClientMaxBodySize &copy) 
: ADirective(copy)
{ *this = copy; }

ClientMaxBodySize::~ClientMaxBodySize() {}

ClientMaxBodySize &ClientMaxBodySize::operator=(const ClientMaxBodySize &other)
{
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_size = other._size;

	return *this;
}

ADirective *ClientMaxBodySize::clone() const
{ return new ClientMaxBodySize(*this); }

unsigned long long	ClientMaxBodySize::getSize() const
{ return _size; }
