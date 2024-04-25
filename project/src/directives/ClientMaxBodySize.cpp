/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:40:36 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 21:27:44 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize()
: ADirective("client_max_body_size", GLOBAL_CONTEXT)
{}

ClientMaxBodySize::ClientMaxBodySize(uint16_t context, size_t size)
: ADirective("client_max_body_size", context), _size(size)
{}

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

size_t	ClientMaxBodySize::getSize() const
{ return _size; }
