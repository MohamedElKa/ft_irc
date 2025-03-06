/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:12:08 by mel-karm          #+#    #+#             */
/*   Updated: 2024/01/07 10:51:13 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Client.hpp"
#include "channel.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>


void      erase_channels(std::vector<Channel> &ch)
{
    for (size_t i  = 0; i < ch.size(); i++)
    {
        if (ch[i].userlist.size() == 0)
            ch.erase(ch.begin() + i);
    }
}

std::string normalize_string(std::string f)
{
    std::string s;
        for (size_t i = 0; i < f.length(); i++)
            s += std::tolower(f[i]);
        size_t index = f.find("\r");
        if (index != std::string::npos)
            s.erase(index);
        index = s.find("\n");
        if (index != std::string::npos)
            s.erase(index);
    return s;
}

void Server::parseCmd(int i)
{
   int index;
   std::string l;
   
   while(1)
   {
        index = clientsList[i].cmd.find_first_of(" ");
        if (index == -1)
        {
            cmds.push_back(clientsList[i].cmd);
            break;
        }
        l = clientsList[i].cmd.substr(0, index);
        if (l != "")
            cmds.push_back(l);
        clientsList[i].cmd = clientsList[i].cmd.substr(index + 1, clientsList[i].cmd.length());
   }
   
}

void Server::readingFromClient()
{
    size_t i;
    std::string getip;
    socklen_t len;
    char str[512];

    memset(str, 0, 512);
    len = sizeof(struct sockaddr);
    i = 0;
    while (i < clientsList.size())
    {
        getpeername(clientsList[i].clientFd, (struct sockaddr *)&clientsList[i].cl, &len);
        clientsList[i].ipaddr = inet_ntoa(clientsList[i].cl.sin_addr);
        if (clientFdlist[i].revents & POLLIN)
        {
            Command _commad;
            Command _check;
            ssize_t rsize = read(clientFdlist[i].fd, str, 512);         
            if (rsize == 0)
            {
                quit(_check, clientsList[i], this->channels);
                clientsList.erase(clientsList.begin() + i);
                clientFdlist.erase(clientFdlist.begin() + i);
            }
            this->clientsList[i].cmd += str;
            _check.parse(this->clientsList[i].cmd.c_str());
            if (_check.command == "quit")
            {
                quit(_check, clientsList[i], this->channels);
                clientsList.erase(clientsList.begin() + i);
                clientFdlist.erase(clientFdlist.begin() + i);
            }
            if (clientsList[i].cmd.find("\r\n") != std::string::npos)
            {
                if (clientsList[i].connected == true)
                {  
                    _commad.parse(this->clientsList[i].cmd.c_str());
                     execute(_commad, clientsList[i]);
                }
                else
                {
                    this->parseCmd(i);
                    this->auth(i);
                }
                this->clientsList[i].cmd = "";
            }
        }
       if (clientFdlist[i].revents & POLLERR)
        {
            close(clientsList[i].clientFd);
            this->clientFdlist.erase(clientFdlist.begin() + i);
            this->clientsList.erase(clientsList.begin() + i);
        }
        i++;
    }

}

Server::Server(std::string port, std::string password)
{
    memset(&this->l, 0, sizeof(struct addrinfo));
    gethostname(this->ip, 100);
    this->serverFd = -1;
    this->password = password;
    this->l.ai_socktype = SOCK_STREAM;
    this->l.ai_family = AF_INET;
    this->l.ai_protocol = 0;
    this->port = port;
    
    std::cout << "server's ip: " << this->ip << std::endl; 
    if (getaddrinfo(this->ip, this->port.c_str(), &l, &p) == -1)
    {
        perror("getaddrinfo info");
        exit(1);
    }
    botIn = 0;
}

void Server::createSocket()
{
    this->serverFd = socket(this->p->ai_family, this->p->ai_socktype, 0);
    if (this->serverFd == -1)
    {
        perror("socket");
        exit (1);
    }
}

void Server::bindingSocket()
{
    if (bind(serverFd, this->p->ai_addr, this->p->ai_addrlen) == -1)
    {
        perror("bind");
        exit(1);
    }
}

void Server::socketOnListening()
{
    if (listen(this->serverFd, 10) == -1)
    {
        perror("listen");
        exit(1);
    }
}


void Server::acceptingClient()
{
   fcntl(serverFd, F_SETFL, O_NONBLOCK);
   while (1)
   {
        int tmpFd = accept(serverFd, p->ai_addr, &p->ai_addrlen); // -1
        if (clientsList.size()  > 50)
        {
            close(tmpFd);
        }
        else{
            if (tmpFd != -1)
            {
                    struct pollfd n;
                    Client nc;
                    nc.clientFd = tmpFd;
                    nc.pass = false;
                    n.fd = tmpFd;
                    fcntl(tmpFd, F_SETFL, O_NONBLOCK);
                    n.events = POLLIN;
                    n.revents = 0;
                    this->clientFdlist.push_back(n);
                    this->clientFd = &clientFdlist[0];
                    clientsList.push_back(nc);
            }        
        }          
        poll(clientFd, clientFdlist.size(), 0);
        this->readingFromClient();
   } 
}

Server::~Server()
{ 
}
std::string Server::check_num(char c, int index)
{
    char str[] = "#0123456789";
    int i;
    std::string error = ERR_ERRONEUSNICKNAME(clientsList[index].ipaddr, "nick");

    i = 0;
    while (i < 11)
    {
        if (str[i] == c)
            return (error);
        i++;            
    }
    return "";
}

std::string Server::checkExnick(std::string nick)
{
    size_t i;

    i = 0;
    while (i < clientsList.size())
    {
        if (clientsList[i].nickName == nick)
            return (ERR_ALREADYREGISTERED(clientsList[i].ipaddr));
        i++;
    }
    return "";
}

void Server::auth(int i)
{
    std::string firstcmd;
    firstcmd = normalize_string(cmds[0]);
    
    if (firstcmd == "pass")
    {
        if (cmds.size() == 2)
        {
            size_t index = cmds[1].find_first_of("\n");
            if (index != std::string::npos)
                cmds[1].erase(index);
            index = cmds[1].find_first_of("\r");
            if (index != std::string::npos)
                cmds[1].erase(index);
            if (cmds[1].compare(password) == 0)
            {
                clientsList[i].pass = true;
            }
            else {
                std::string error;

                error =   ERR_PASSWDMISTACH(clientsList[i].ipaddr);
                write(clientsList[i].clientFd, error.c_str(), error.length());
            }
        }
        else {
            std::string error;

            error = ERR_NEEDMOREPARAMS(clientsList[i].ipaddr, "pass");
            write(clientsList[i].clientFd, error.c_str(), error.length());
        }
    }
    else if (firstcmd == "nick")
    {
        if (cmds.size() >= 2)
        {
            if (clientsList[i].pass == true)
            {
                size_t in = cmds[1].find_last_of("\r");
                if (in != std::string::npos)
                    cmds[1].erase(in);
                in = cmds[1].find_last_of("\n");
                if (in != std::string::npos)
                    cmds[1].erase(in);
                std::string s = check_num(cmds[1][0], i);
                if (s.length() != 0)
                {
                    cmds.clear();
                    return;        
                }
                s = checkExnick(this->cmds[1]);
                if (s.length() != 0)
                {
                    close(clientsList[i].clientFd);
                    clientsList.erase(clientsList.begin() + i);
                    clientFdlist.erase(clientFdlist.begin() + i);
                    write(clientsList[i].clientFd, s.c_str(), s.length());
                }
                else
                    clientsList[i].nickName = cmds[1].substr(0, 15);
            }
            else{
               
            }
        }
        else{
            // std::string error;

            // error = ERR_NEEDMOREPARAMS(clientsList[i].ipaddr, "nick");
            // write(clientsList[i].clientFd, error.c_str(), error.length());
            
        }
    }
    else if (firstcmd == "user")
    {
        if (cmds.size() >= 5)
        {
            if (cmds[2] == "0" && cmds[3] == "*" && clientsList[i].pass == true)
            {
                clientsList[i].userName = cmds[1];
                size_t j;

                j = 4;
                while (j < cmds.size())
                {
                    clientsList[i].realName += cmds[j];
                    j++;
                }
            }
            else{
            }
        }
        else {
            std::string error;

            error = ERR_NEEDMOREPARAMS(clientsList[i].ipaddr, "user");
            write(clientsList[i].clientFd, error.c_str(), error.length());
        }
    }
    if (clientsList[i].pass == true && clientsList[i].nickName.empty() == false && clientsList[i].userName.empty() == false)
    {
        write_to_client(clientsList[i].clientFd, RPL_WELCOME(clientsList[i].nickName));
        write_to_client(clientsList[i].clientFd, RPL_YOURHOST(clientsList[i].nickName, myhostname()));
        write_to_client(clientsList[i].clientFd, RPL_CREATED(clientsList[i].nickName));
        write_to_client(clientsList[i].clientFd, RPL_ISUPPORT(clientsList[i].nickName, "MAXCHANNEL=20 MAXCLIENT=51 MAXNICKLEN=15 MAXTOPICLEN=60"));
        write_to_client(clientsList[i].clientFd, RPL_MOTD(clientsList[i].nickName, "This server accepts IPv4 connections on the following ports: 2000-9000"));
        clientsList[i].connected = true;
    }
    cmds.clear();
}