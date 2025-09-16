/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/14 15:19:10 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include "Channel.hpp"

class Client
{
    private:
        std::string             _username;
        std::string             _nickname;
        std::vector<Channel>    _channels; 

    public:
        Client(std::string usr, std::string nick);
    
        std::string             getNickname() const;
        std::string             getUsername() const;
        std::vector<Channel>    getChannels() const; 
        
        void                    joinChannel(size_t channelId);
        void                    quitChannel(size_t channelId);
        void                    sendMessage(size_t clientId); // ou channel id
};
