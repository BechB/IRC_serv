#include "Client.hpp"

Client::Client(std::string usr, std::string nick) : _username(usr), _nickname(nick) {}

std::string Client::getUsername() const
{
    return _username;
}

std::string Client::getNickname() const
{
    return _nickname;
}

std::vector<Channel> Client::getChannels() const
{
    return _channels;
}

void Client::joinChannel(std::string channelName)
{
    for (size_t i = 0; i < )
}