#include "channel.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include "command.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

void    write_to_client(int _fd, const std::string &msg)
{
    send(_fd, msg.c_str(), msg.size(), 0);
}  

std::string myhostname(void)
{
    std::string host_name;
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        host_name = hostname;
        return host_name;
    } else {
        perror("Error getting hostname");
    }
    return host_name;
}

bool    is_empty(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isspace(static_cast<unsigned char>(str[i])))
            return false;
    }
    return true;
}

bool    is_digit(const char *str)
{
    if (!str)
        return false;
    for (size_t i = 0; str[i]; ++i)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

std::vector<Client>::iterator check_client_in_server(Client &client, std::vector<Client> &clients)
{
	std::vector<Client>::iterator itclient = std::find(clients.begin(), clients.end(), client);
    if (itclient != clients.end())
        return itclient;
    return clients.end();
}

std::string toString(int n)
{
    std::stringstream ss;
    ss << n;
    std::string strNumber = ss.str();
    return strNumber;
}
