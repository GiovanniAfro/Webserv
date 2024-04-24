/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-nata <adi-nata@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:44:51 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/24 14:51:27 by adi-nata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

enum LOCATION_MODIFIER
{
	NONE,					// No modifier
	EXACT,					// '=' Exact match
	CASE_SENSITIVE_REGEX,	// '~' Case-sensitive regular expression match
	CASE_INSENSITIVE_REGEX,	// '~*' Case-insensitive regular expression match
	LONGEST_NON_REGEX,		// '^~' Longest non-regular expression match
	INVALID
};
extern std::vector<LOCATION_MODIFIER>	allLocationModifiers;

// Class ---------------------------------------------------------------------->

/*!
 * @ref
	Docs:       https://nginx.org/en/docs/http/ngx_http_core_module.html#location
	Syntax:	    location [ = | ~ | ~* | ^~ ] uri { ... }
				location @name { ... }
	Default:	—
	Context:	server, location
 */
class	Location : public ADirective {
	private:
		enum LOCATION_MODIFIER	_modifier;
		std::string	_uri;

		Location();

	public:
		Location(uint16_t context, enum LOCATION_MODIFIER modifier, const std::string& uri);
		Location(const Location &copy);
		~Location();
		Location &operator=(const Location &other);
		ADirective *clone() const;

		enum LOCATION_MODIFIER	getModifier() const;
		const std::string&	getUri() const;

		// Utils -------------------------------------------------------------->
		static enum LOCATION_MODIFIER	_modifierToEnum(const std::string& modifier);
		static std::string				_modifierToString(enum LOCATION_MODIFIER modifier);

};
