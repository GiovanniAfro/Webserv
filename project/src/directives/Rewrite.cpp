/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rewrite.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:46:49 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/22 10:47:08 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Rewrite.hpp"

Rewrite::Rewrite() : ADirective("Rewrite", GLOBAL_CONTEXT) {}

Rewrite::Rewrite(uint16_t context) : ADirective("Rewrite", context) {}

Rewrite::Rewrite(const Rewrite &copy) : ADirective(copy) {
	*this = copy;
}

Rewrite::~Rewrite() {}

Rewrite &Rewrite::operator=(const Rewrite &other) {
	if (this == &other)
		return *this;

	ADirective::operator=(other);
	return *this;
}

ADirective *Rewrite::clone() const {
	return new Rewrite(*this);
}
