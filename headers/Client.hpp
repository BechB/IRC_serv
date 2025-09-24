/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/23 17:52:40 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include <set>
#include <vector>


class Client
{
    private:
        int                         _fd;
        bool                        _hasPass;
        bool                        _hasNick;
        bool                        _hasUser;
        bool                        _isRegistred;
        std::string                 _username;
        std::string                 _nickname;
        std::set<std::string>       _channels;

    public:
        Client(int fd);

        // setters
        void    setHasPass();
        void    setHasNick();
        void    setHasUser();
        void    setNickname(const std::string& nickname);
        void    setUsername(const std::string& username);
        
        // getters
        int                         getFd() const;
        bool                        getIsRegistred() const;
        bool                        getHasPass() const;
        bool                        getHasNick() const;
        bool                        getHasUser() const;
        std::string                 getNickname() const;
        std::string                 getUsername() const;
        std::set<std::string>       getChannels() const;

        // members
        void    closeFd();
        void    joinChannel(const std::string& name);
        void    quitChannel(const std::string& name);
};
