/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/05 16:33:09 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include <set>
#include <vector>
#include <unistd.h> // close
#include <iostream>

class Client
{
    private:
        int                         _fd;
        bool                        _hasPass;
        bool                        _hasNick;
        bool                        _hasUser;
        bool                        _registred;
        std::string                 _username;
        std::string                 _nickname;
        std::string                 _oldNickname;
        std::set<std::string>       _channels;

    public:
        Client(int fd);

        // setters
        void    setHasPass();
        void    setHasNick();
        void    setHasUser();
        void    setRegistred();
        void    setNickname(const std::string& nickname);
        void    setUsername(const std::string& username);

        // getters
        int                         getFd() const;
        bool                        getIsRegistred() const;
        bool                        getHasPass() const;
        bool                        getHasNick() const;
        bool                        getHasUser() const;
        std::string                 getNickname() const;
        std::string                 getOldNickname() const;
        std::string                 getUsername() const;
        std::set<std::string>       getChannels() const;

        // members
        void    closeFd();
        void    joinChannel(const std::string& name);
        void    leaveChannel(const std::string& name);
};
