/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:41:48 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/23 22:03:16 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
: ADirective("error_page", GLOBAL_CONTEXT), _response(NOT_FOUND)
{}

ErrorPage::ErrorPage(uint16_t context, const std::vector<enum HTTP_STATUS> &codes, enum HTTP_STATUS response, const std::string &uri)
: ADirective("error_page", context), _codes(codes), _response(response), _uri(uri)
{
	bool	codesOk = false, responseOk = false;

	for (std::vector<HTTP_STATUS>::iterator itCodes = _codes.begin(); itCodes != _codes.end(); ++itCodes)
	{
		for (std::vector<HTTP_STATUS>::iterator it = allHttpStatus.begin(); it != allHttpStatus.end(); ++it)
		{
			if (*itCodes == *it)
				codesOk = true;
			if (response == *it)
				responseOk = true;
		}
		if (!codesOk)
			throw std::runtime_error("error_page : invalid code");
	}
	if (!responseOk)
		throw std::runtime_error("error_page : invalid response code");
}

ErrorPage::ErrorPage(const ErrorPage &copy) : ADirective(copy) {
	*this = copy;
}

ErrorPage::~ErrorPage() {}

ErrorPage &ErrorPage::operator=(const ErrorPage &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	this->_codes = other._codes;
	this->_response = other._response;
	this->_uri = other._uri;

	return *this;
}

ADirective *ErrorPage::clone() const {
	return new ErrorPage(*this);
}

std::vector<enum HTTP_STATUS> &ErrorPage::getCodes() {
	return _codes;
}

enum HTTP_STATUS	ErrorPage::getResponse() {
	return _response;
}

const std::string &ErrorPage::getUri() const {
	return _uri;
}
