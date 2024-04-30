/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kichkiro <kichkiro@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:25:02 by kichkiro          #+#    #+#             */
/*   Updated: 2024/04/30 22:26:46 by kichkiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.h"

// Class ---------------------------------------------------------------------->

class Cgi {
    private:
        std::map<std::string,std::string> _params;

    public:
        Cgi(std::map<std::string, std::string> &req, std::map<std::string, 
            std::string> req_head, std::string path_info);
		Cgi(const Cgi &copy);
		~Cgi();
		Cgi &operator=(const Cgi &other);

        std::string exec(void);
};
