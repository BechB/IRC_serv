/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/18 15:10:38 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>
#include "Channel.hpp"
#include <unistd.h>   // pour close()

class Channel;

class Client
{
    private:
        int                     _fd;

        bool                    _hasPass;
        bool                    _hasNick;
        bool                    _hasUser;
        bool                    _registred;

        std::string             _username;
        std::string             _nickname;
        std::vector<Channel>    _channels;

    public:
        Client(int fd);
    
        void                    setHasPass();
        void                    setHasNick();
        void                    setHasUser();
        void                    setRegistred();
        void                    setNickname(std::string& nickname);
        void                    setUsername(std::string& username);

        int                     getFd() const;
        bool                    getHasPass() const;
        bool                    getHasNick() const;
        bool                    getHasUser() const;
        bool                    getRegistred() const;
        bool                    isRegistred();
        std::string             getNickname() const;
        std::string             getUsername() const;
        std::vector<Channel>    getChannels() const;

        void                    closeFd();
};
