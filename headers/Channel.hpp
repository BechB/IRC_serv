/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldalmas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:59:23 by aldalmas          #+#    #+#             */
/*   Updated: 2025/09/27 18:35:36 by aldalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>


class Channel
{
    private:
        int             _memberLimit;
        bool            _invitOnly;
        std::string     _key;
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

        // members
        bool            checkKey(const std::string& key) const;
        void            addMember(int client_fd);
        void            addOperator(int client_fd);
        void            removeMember(int client_fd);
        void            removeOperator(int client_fd);
};
