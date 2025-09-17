#include "headers/Client.hpp"


Client::Client(int fd) : _fd(fd) {}


// setters
void Client::setNickname(std::string& nickname) {_nickname = nickname;}

void Client::setUsername(std::string& username) {_username = username;}

void Client::closeFd() {close(_fd);}

// getters
int Client::getFd() const {return _fd;}

std::string Client::getUsername() const {return _username;}

std::string Client::getNickname() const {return _nickname;}

std::vector<Channel> Client::getChannels() const {return _channels;}