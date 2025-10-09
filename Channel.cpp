/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:01:24 by bbousaad          #+#    #+#             */
/*   Updated: 2025/10/09 11:52:53 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"
#include <iostream>


Channel::Channel(const std::string &name, int creator_fd)
{
    _memberLimit = -1;
    _invitOnly = false;
    _topicRestricted = true;
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
void Channel::removeInvitOnly() {_invitOnly = false;}


// getters
int  Channel::getMemberLimit() const {return _memberLimit;}
bool Channel::getInvitOnly() const {return _invitOnly;}
bool Channel::getTopicRestriction() const {return _topicRestricted;}

std::string Channel::getKey() const {return _key;}
std::string Channel::getTopic() const {return _topic;}
std::string Channel::getName() const {return _name;}
std::set<int> Channel::getMembers() const {return _members;}
std::set<int> Channel::getOperators() const {return _operators;}



// members
bool Channel::isOperator(int fd) const {return _operators.find(fd) != _operators.end();}
bool Channel::checkKey(const std::string& key) const {return _key == key;}
void Channel::addMember(int client_fd) {_members.insert(client_fd);}
void Channel::addOperator(int client_fd) {_operators.insert(client_fd);}
void Channel::removeMember(int client_fd) {_members.erase(client_fd);}
void Channel::removeOperator(int client_fd) {_operators.erase(client_fd);}

void Channel::broadcast(const std::string& msg, int except_fd) const
{
    for (std::set<int>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it == except_fd)
            continue;
        
        send(*it, msg.c_str(), msg.size(), 0);
    }
}