/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/26 16:51:13 by aldalmas         ###   ########.fr       */
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
        bool                        _isQuiting;
        bool                        _hexchatSignedIn;
        std::string                 _username;
        std::string                 _nickname;
        std::string                 _oldNickname;
        std::string					_incompleteMsg; // ctrl D
        std::set<std::string>       _channels;

    public:
        Client(int fd);

        // setters
        void    setHasPass();
        void    setHasNick();
        void    setHasUser();
        void    setRegistred();
        void    setHexchatSignedIn();
        void    setIsQuiting();
        void    setNickname(const std::string&);
        void    setUsername(const std::string&);

        // getters
        int                         getFd() const;
        bool                        getIsRegistred() const;
        bool                        getHasPass() const;
        bool                        getHasNick() const;
        bool                        getHasUser() const;
        bool                        getHexchatSignedIn() const;
        std::string                 getNickname() const;
        std::string                 getOldNickname() const;
        std::string                 getUsername() const;
        std::string                 getIncompleteMsg() const;
        std::set<std::string>       getChannels() const;
        bool                        getIsQuiting() const;

        // members
        void    closeFd();
        void    joinChannel(const std::string&);
        void    leaveChannel(const std::string&);
        void    updateIncompMsg(const std::string&);
        void    clearIncompMsg();
};
