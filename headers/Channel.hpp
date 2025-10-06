/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <aldalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:59:23 by aldalmas          #+#    #+#             */
/*   Updated: 2025/10/06 18:49:39 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>


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

    public:
        Channel(const std::string &name, int creator_fd);

        // setters
        void            setTopic(const std::string& newTopic);
        void            setKey(const std::string& newKey);
        void            setLimit(int limit);
        void            setInvitOnly();
        
        // specialized setters
        void            enableInvitOnly();
        void            removeInvitOnly();

        // getters
        int             getMemberLimit() const;
        bool            getInvitOnly() const;
        std::string     getKey() const;
        std::string     getTopic() const;
        std::string     getName() const;
        std::set<int>   getMembers() const;
        std::set<int>   getOperators() const;
        bool            getTopicRestriction() const;

        // members
        bool            checkKey(const std::string& key) const;
        void            addMember(int client_fd);
        void            addOperator(int client_fd);
        void            removeMember(int client_fd);
        void            removeOperator(int client_fd);
};
