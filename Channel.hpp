#pragma once


class Channel
{
    private:
        std::string         _name;
        std::vector<Client> _clients_id;

    public:
        std::string         getName() const;
        std::vector<Client> getClientsId() const;

};