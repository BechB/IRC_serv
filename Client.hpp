/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:21:28 by bbousaad          #+#    #+#             */
/*   Updated: 2025/08/10 15:22:43 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <poll.h>


class Client
{
    private:

    public:
        int fd;
        bool isRegistered;
        std::string nickname;
        std::string username;
        std::vector<int> currentChannels;
        
        void setUsername(std::string& username);
        void setNickname(std::string& nickname);
        std::string getUsername();
        std::string getNickname();
        
        // COMMANDS
        void joinChannel(std::string& channelName);
        void sendMsg();
        void sendPrivateMsg();
        void quitChannel();
};