#include "headers/Client.hpp"


Client::Client(int fd)
{
    _fd = fd;
    _hasPass = false;
    _hasNick = false;
    _hasUser = false;
    _registred = false;
}


// setters
void Client::setHasPass() {_hasPass = true;}
void Client::setHasUser() {_hasUser = true;}
void Client::setHasNick() {_hasNick = true;}
void Client::setRegistred() {_registred = true;}
void Client::setNickname(std::string& nickname) {_nickname = nickname;}
void Client::setUsername(std::string& username) {_username = username;}
void Client::closeFd() {close(_fd);}

// getters
int Client::getFd() const {return _fd;}
bool Client::getHasPass() const {return _hasPass;}
bool Client::getHasNick() const {return _hasNick;}
bool Client::getHasUser() const {return _hasUser;}
bool Client::getRegistred() const {return _registred;}

bool Client::isRegistred()
{
    if (_hasPass && _hasNick && _hasUser)
    {
        _registred = true;
        return true;
    }

    return false;
}

std::string Client::getUsername() const {return _username;}
std::string Client::getNickname() const {return _nickname;}
std::vector<Channel> Client::getChannels() const {return _channels;}
