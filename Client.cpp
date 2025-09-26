/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:34:55 by aldalmas          #+#    #+#             */
/*   Updated: 2025/09/26 12:40:29 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"
#include <unistd.h> // close
#include <iostream>

test
Client::Client(int fd)
{
    _fd = fd;
    _hasPass = false;
    _hasNick = false;
    _hasUser = false;
}


// setters
void                        Client::setHasPass() {_hasPass = true;}
void                        Client::setHasUser() {_hasUser = true;}
void                        Client::setHasNick() {_hasNick = true;}
void                        Client::setNickname(const std::string& nickname) {_nickname = nickname;}
void                        Client::setUsername(const std::string& username) {_username = username;}


// getters
int                         Client::getFd() const {return _fd;}
bool                        Client::getHasPass() const {return _hasPass;}
bool                        Client::getHasNick() const {return _hasNick;}
bool                        Client::getHasUser() const {return _hasUser;}
bool                        Client::getIsRegistred() const {return _hasPass && _hasNick && _hasUser;}
std::string                 Client::getUsername() const {return _username;}
std::string                 Client::getNickname() const {return _nickname;}
std::set<std::string>       Client::getChannels() const {return _channels;}


// members
void                        Client::closeFd() {close(_fd);}
void                        Client::joinChannel(const std::string& name) {_channels.insert(name);}
void                        Client::quitChannel(const std::string& name) {_channels.erase(name);}