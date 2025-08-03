/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbousaad <bbousaad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:02 by bbousaad          #+#    #+#             */
/*   Updated: 2025/07/21 13:27:32 by bbousaad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>

class Server
{
    private:

    std::string _password;
    int _port;

    public:
        Server();
        ~Server();
    
};