/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:22:21 by simo              #+#    #+#             */
/*   Updated: 2023/12/31 13:35:10 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include <stdio.h>
#include <time.h>

#define PASS 1
#define NICK 2
#define USER 3
#define WRONGPASS   "<client> :Password incorrect"

std::string myhostname( void );

#define BROAD_MODE 0
#define BROAD_KICK 1
#define BROAD_JOIN 2
#define BROAD_QUIT 3
#define BROAD_NICK 4
#define BROAD_PVMSG 5
#define BROAD_PART 6
#define BROAD_TOPIC 7
#define BROAD_TOPICTIME 8
#define BROAD_PRIVMSG 9

#define MY_HOSTNAME ":" + myhostname()

#define ERR_ALREADYREGISTERED(client) MY_HOSTNAME + " 462 " +  (client) + " " + " :You may not reregister\r\n"
#define ERR_ERRONEUSNICKNAME(client, command) MY_HOSTNAME + " 432 " +  (client) + " " + command + " ::Erroneus nickname\r\n"
#define ERR_COMMANDNOTFOUND(client, command) MY_HOSTNAME + " 421 " +  (client) + " " + (command) + " :Unknown command\r\n"
#define ERR_NOSUCHNICK(client1, client2) MY_HOSTNAME + " 401 " + (client1) + " " + (client2) + " :No such nick\r\n"
#define ERR_NEEDMOREPARAMS(client, command) MY_HOSTNAME +  " 461 " + (client) + " " + (command) + " :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL(client, channel)  MY_HOSTNAME + " 403 " + (client) + " " + (channel) + " :No such channel\r\n"
#define ERR_BADCHANNELKEY(client, channel)   MY_HOSTNAME + " 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)\r\n"
#define ERR_NOTONECHANNEL(client, channel)  MY_HOSTNAME + " 442 " + (client) + " " + (channel) + " :You're not on that channel\r\n"
#define ERR_CHANOPRIVSNEEDED(client, channel) MY_HOSTNAME +  " 482 " + (client) + " " + (channel) + " :You're not channel operator\r\n"
#define ERR_PASSWDMISTACH(client) MY_HOSTNAME + " 464 " + (client) + " :Password incorrect\r\n"
#define ERR_USERONCHANNEL(client, nick, channel) MY_HOSTNAME + " 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel\r\n"
#define ERR_INVITEONLYCHAN(client, channel) MY_HOSTNAME + " 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANMASK(channel)  MY_HOSTNAME + " 476 " + (channel) + " :Bad Channel Mask\r\n"
#define ERR_UNKNOWNMODE(client, modechar) MY_HOSTNAME + " 472 " + (client) + " " + (modechar) + " :is unknown mode char to me\r\n"
#define ERR_TOOMANYCHANNELS(client, channel) MY_HOSTNAME +  " 405 " + (client) + " "  + (channel) + " :You have joined too many channels\r\n"
#define ERR_CHANNELISFULL(client, channel)  MY_HOSTNAME + " 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)\r\n"
#define ERR_KEYALREADYSET(client, channel) MY_HOSTNAME + " 467 " + (client) + " " + (channel) + " :Channel key already set\r\n"
#define ERR_NORECIPENT(client, command) MY_HOSTNAME + " 411 " + (client) + " :No recipient given " + (command) + "\r\n"
#define ERR_NOGIVENNICK(client)  MY_HOSTNAME + " 431 " + (client) + " :No nickname given\r\n"
#define ERR_ONEOUS(client, wrongnick) MY_HOSTNAME  +" 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname\r\n"
#define ERR_NICKINUSE(client, usednick) MY_HOSTNAME  + " 433 " + (client) +  " ana :Nickname is already in use.\r\n"
#define ERR_NOTEXTTOSEND(client) MY_HOSTNAME + " 412 " + (client) + " :No text to send\r\n"
#define ERROR_MSG(reason) MY_HOSTNAME + " " + (reason) + "\r\n"

#define NICK_MSG(client, username,ipaddr, newnick)  ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick) + "\r\n"
#define KICK_MSG(client, ipaddr, channel, kicked, reason) ":" + (client) + "!~sad@" + (ipaddr) + " KICK " + (channel) + " " + kicked + " " + reason + "\r\n"
#define QUIT_MSG(nickname, username, ipaddr, reason) ":"+ (nickname) + "!~" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason) + "\r\n"
#define PRIVMSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
#define JOIN_SUCC(nickname, username, client_ip, channel) ":" + (nickname) + "!~" + (username) + "@" + (client_ip) + " JOIN " + (channel) + "\r\n"
#define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target + "\r\n"
#define PART_MSG(nickname, username, ipaddr, channel, reason) ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason) + "\r\n"
#define NOTICE_MSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message) + "\r\n"

#define RPL_NAMERPLY(client, channel, users_list) MY_HOSTNAME + " 353 " + (client) + " = " + (channel) + " :" + (users_list) + "\r\n"
#define RPL_CREATIONTIME(client, channel, creation_time) MY_HOSTNAME + " 329 " + (client) + " " + (channel) + " " + (creation_time) + "\r\n"
#define RPL_ENDOFNAMES(client, channel)  MY_HOSTNAME + " 366 " + (client) + " " + (channel) + " :End of /NAMES list\r\n"
#define RPL_ENDOFINVITELIST(client) MY_HOSTNAME + " 337 " + " :End of /INVITE list\r\n"
#define RPL_CHANNELMODEIS(client, channel, modestring) MY_HOSTNAME + " 324 " + (client) + " "+ (channel) + " " +(modestring) + " \r\n"
#define RPL_TOPIC(client, channel, topic) MY_HOSTNAME + " 332 " + (client) + " " + (channel) +  " :" + (topic) + "\r\n"
#define RPL_NOTOPIC(client, channel)  MY_HOSTNAME + " 331 " + (client) + " " + (channel) + " :No topic is set\r\n"
#define RPL_INVITING(client, nick, channel)  MY_HOSTNAME + " 341 " + (client) + " " + (nick) + " " + (channel) + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, setat) MY_HOSTNAME + " 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\r\n"
#define RPL_WELCOME(client) MY_HOSTNAME +  " 001 " + (client) + " :Welcome to the IRC Network, " + (client) + "\r\n"
#define RPL_YOURHOST(client, hostname) MY_HOSTNAME + " 002 " + (client) + " :Your host is " + (hostname) + "\r\n"
#define RPL_CREATED(client) MY_HOSTNAME + " 003 " + (client) + " :This server was created Mon Dec 26 2023 at 11:33:05 EDT\r\n"
#define RPL_ISUPPORT(client, values)  MY_HOSTNAME + " 005 " + (client) + " " + (values) + " :are supported by this server\r\n"
#define RPL_MOTD(client, sentence) MY_HOSTNAME + " 372 " + (client) + " " + (sentence) + "\r\n"

int         server(std::string password, std::string port);
bool        is_empty(const std::string& str);
bool        is_digit(const char *str);
void        Launch(std::string port, std::string password);
std::string	getsetat();

#endif