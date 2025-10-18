/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:59:23 by aldalmas          #+#    #+#             */
/*   Updated: 2025/10/18 19:54:26 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>
#include <sys/socket.h>
#include <sstream>

class Channel
{
    private:
        int             _memberLimit;               // +l
        bool            _invitOnly;                 // +i
        bool            _topicRestricted;           // +t
        std::string     _key;                       // +k
        std::string     _topic;
        std::string     _name;
        std::set<int>   _operators;
        std::set<int>   _members;
        std::set<int>   _guests;

    public:
        Channel(const std::string &name, int creator_fd);

        // setters
        void            setTopic(const std::string& newTopic);
        void            setKey(const std::string& newKey);
        void            setLimit(int limit);
        void            setInvitOnly();
        
        // specialized setters
        void            enableInvitOnly();
        void            enableTopicRestriction();
        void            removeInvitOnly();
        void            removeTopicRestriction();
        void            removeLimit();

        // getters
        int             getMemberLimit() const;
        bool            getInvitOnly() const;
        bool            getTopicRestriction() const;
        std::string     getKey() const;
        std::string     getTopic() const;
        std::string     getName() const;
        std::string     getModes() const;
        std::set<int>   getMembers() const;
        std::set<int>   getOperators() const;
        std::set<int>   getGuest() const;

        // members
        bool            isOperator(int client_fd) const;
        bool            isMember(int client_fd) const;
        bool            isGuest(int client_fd) const;
        bool            checkKey(const std::string& key) const;
        void            addMember(int client_fd);
        void            addOperator(int client_fd);
        void            addGuest(int client_fd);
        void            removeMember(int client_fd);
        void            removeOperator(int client_fd);
        void            removeGuest(int client_fd);
        void            broadcast(const std::string& message, int except_fd = -1) const;
};


