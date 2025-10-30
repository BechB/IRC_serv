/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:34:55 by aldalmas          #+#    #+#             */
/*   Updated: 2025/10/30 15:14:36 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"



Client::Client(int fd)
{
    _fd = fd;
    _hasPass = false;
    _hasNick = false;
    _hasUser = false;
    _registred = false;
    _isQuiting = false;
    _hexchatSignedIn = false;
}


// setters
void    Client::setHasPass() {_hasPass = true;}
void    Client::setHasUser() {_hasUser = true;}
void    Client::setHasNick() {_hasNick = true;}
void    Client::setRegistred() {_registred = true;}
void    Client::setHexchatSignedIn() {_hexchatSignedIn = !_hexchatSignedIn;}
void    Client::setIsQuiting() {_isQuiting = true;}
void    Client::setUsername(const std::string& username) {_username = username;}
void    Client::setNickname(const std::string& nickname)
{
    _oldNickname = _nickname;
    _nickname = nickname;
}


// getters
int                     Client::getFd() const {return _fd;}
bool                    Client::getHasPass() const {return _hasPass;}
bool                    Client::getHasNick() const {return _hasNick;}
bool                    Client::getHasUser() const {return _hasUser;}
bool                    Client::getIsQuiting() const {return _isQuiting;}
bool                    Client::getHexchatSignedIn() const {return _hexchatSignedIn;}
bool                    Client::getIsRegistred() const {return _registred;}
std::string             Client::getUsername() const {return _username;}
std::string             Client::getNickname() const {return _nickname;}
std::string             Client::getIncompleteMsg() const {return _incompleteMsg;}
std::set<std::string>   Client::getChannels() const {return _channels;}
std::string             Client::getOldNickname() const {return _oldNickname;}


// members
void    Client::closeFd() {close(_fd);}
void    Client::joinChannel(const std::string& name) {_channels.insert(name);}
void    Client::leaveChannel(const std::string& name) {_channels.erase(name);}
void    Client::updateIncompMsg(const std::string& msgPart) {_incompleteMsg += msgPart;}
void    Client::clearIncompMsg() {_incompleteMsg.clear();}
