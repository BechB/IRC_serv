/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:01:24 by bbousaad          #+#    #+#             */
/*   Updated: 2025/09/23 18:48:42 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"
#include <iostream>


Channel::Channel(const std::string &name, int creator_fd)
{
    _memberLimit = -1;
    _invitOnly = false;
    _key = "";
    _topic = "";
    _name = name;
    _operators.insert(creator_fd);
    _members.insert(creator_fd);
}

int                 Channel::getMemberLimit() const {return _memberLimit;}
bool                Channel::getInvitOnly() const {return _invitOnly;}
std::string         Channel::getKey() const {return _key;}
std::string         Channel::getTopic() const {return _topic;}
std::string         Channel::getName() const {return _name;}
std::set<int>       Channel::getMembers() const {return _members;}
std::set<int>       Channel::getOperators() const {return _operators;}

void                Channel::addMember(int client_fd);