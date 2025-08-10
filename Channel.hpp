/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:07 by bbousaad          #+#    #+#             */
/*   Updated: 2025/08/10 15:21:42 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include <limits>
#include <cerrno>
#include <vector>
#include "Client.hpp"


class Channel 
{
    private:

    public:
        std::string name;
        std::string topic;
        std::vector<Client> clientList;

        void setName(std::string& name);
        void setTopic(std::string& topic);
        
        std::string getName();
        std::string getTopic();

        void addClient(Client& client);

        // COMMANDS
        void kick(Client& client);
        void invite(Client& client);
        void topic(std::string& topic);
        void mode(std::string& mode);
};