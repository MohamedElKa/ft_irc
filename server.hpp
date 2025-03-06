/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:12:10 by mel-karm          #+#    #+#             */
/*   Updated: 2024/01/07 10:44:10 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "channel.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include "command.hpp"
#include <ctime>
#include <string>
#include <vector>

class Server{
    public:
        /* Attributes */
        time_t pingTime;
        int             serverFd;
        // std::string     sever_ip;
        std::string     cmd;
        std::string     password;
        std::string     port;
        struct addrinfo l;
        struct addrinfo *p;
        struct pollfd   *clientFd;
        int botIn;
        char ip[100];
        
        std::vector<Channel>        channels;
        std::vector<class Client>   clientsList;
        std::vector<std::string>    cmds;        
        std::vector<struct pollfd>  clientFdlist;        
        /* Methods */ 
        
        Server(std::string port, std::string password);
        ~Server();
        void    createSocket();
        void    bindingSocket();
        void    socketOnListening();
        void    acceptingClient();
        void    readingFromClient();
        void parseCmd(int i);
        void    auth(int i);
        std::string check_num(char c, int index);
        void    execute(Command &cmd, Client &user);
        std::string    checkExnick(std::string nick);
};

#endif 