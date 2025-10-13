/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:01:24 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/13 18:09:23 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"
#include <iostream>


Channel::Channel(const std::string &name, int creator_fd)
{
    _memberLimit = -1;
    _invitOnly = false;
    _topicRestricted = false;
    _key = "";
    _topic = "";
    _name = name;
    _operators.insert(creator_fd);
    _members.insert(creator_fd);
}

// setters
void Channel::setTopic(const std::string& newTopic) {_topic = newTopic;}
void Channel::setKey(const std::string& newKey) {_key = newKey;}
void Channel::setLimit(int limit) {_memberLimit = std::max(1, std::min(limit, 100));}
void Channel::setInvitOnly() {_invitOnly = true;}

// specialized setters
void Channel::enableInvitOnly() {_invitOnly = true;}
void Channel::enableTopicRestriction() {_topicRestricted = true;}

void Channel::removeInvitOnly() {_invitOnly = false;}
void Channel::removeTopicRestriction() {_topicRestricted = false;}
void Channel::removeLimit() {_memberLimit = -1;}

// getters
int  Channel::getMemberLimit() const {return _memberLimit;}
bool Channel::getInvitOnly() const {return _invitOnly;}
bool Channel::getTopicRestriction() const {return _topicRestricted;}

std::string Channel::getKey() const {return _key;}
std::string Channel::getTopic() const {return _topic;}
std::string Channel::getName() const {return _name;}
std::string Channel::getModes() const
{
    std::string modes = "+";
    std::string args = "";

    if (_invitOnly)
        modes += "i";

    if (_topicRestricted)
        modes += "t";

    if (!_key.empty())
    {
        modes += "k";    
        args += _key + " ";
    }

    if (_memberLimit > 0)
    {
        modes += "l";
        std::stringstream iss;
        iss << _memberLimit;
        std::string converted = iss.str();
        args += converted;   
    }

    return modes + " " + args;
}

std::set<int> Channel::getMembers() const {return _members;}
std::set<int> Channel::getOperators() const {return _operators;}
std::set<int> Channel::getInvited() const {return _invited;}


// members
bool Channel::isOperator(int client_fd) const {return _operators.find(client_fd) != _operators.end();}
bool Channel::isMember(int client_fd) const {return _members.find(client_fd) != _members.end();}
bool Channel::isInvited(int client_fd) const {return _invited.find(client_fd) != _invited.end();}
bool Channel::checkKey(const std::string& key) const {return _key == key;}
void Channel::addMember(int client_fd) {_members.insert(client_fd);}
void Channel::addOperator(int client_fd) {_operators.insert(client_fd);}
void Channel::addInvited(int client_fd) {_invited.insert(client_fd);}
void Channel::removeMember(int client_fd) {_members.erase(client_fd);}
void Channel::removeOperator(int client_fd) {_operators.erase(client_fd);}
void Channel::removeInvited(int client_fd) {_invited.erase(client_fd);}

void Channel::broadcast(const std::string& msg, int except_fd) const
{
    for (std::set<int>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it == except_fd)
            continue;
        
        send(*it, msg.c_str(), msg.size(), 0);
    }
}