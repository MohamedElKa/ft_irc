/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:12:04 by mel-karm          #+#    #+#             */
/*   Updated: 2024/01/05 17:42:12 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ft_irc.hpp"
#include <netdb.h>
#include <netinet/in.h>

class Client
{
public:
    std::string nickName;
    std::string userName;
    std::string realName;
    std::string cmd;
    int clientFd;
    bool pass;
    bool connected;
    struct sockaddr_in cl;
    char *ipaddr;

    Client(){
        nickName = "";
        userName = "";
        pass = false;
        connected = false;
        clientFd = -1;
    }
    Client(const std::string &s)
    {
        nickName = s;
    }
    bool    operator==(const Client &c) const
    {
        if (this->nickName == c.nickName)
            return true;
        return false;
    }
     bool operator()(const Client& client) const {
        return client.nickName == this->nickName;
    }
    ~Client(){
        // delete ipaddr;
    }
};


#endif