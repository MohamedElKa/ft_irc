/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:34:43 by mel-karm          #+#    #+#             */
/*   Updated: 2024/01/06 13:49:54 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <cstdio>
#include <netdb.h>
#include <fstream>

#include <sys/fcntl.h>
#include <sys/socket.h>
#include "ft_irc.hpp"
#include "command.hpp"

void bot(Client &user)
{
    int i = 0;
    static int ch;
    std::string joke[35];
    std::ifstream file("data");
   
    if (!file.is_open())
        return ;
    while (i < 35)
    {
        std::getline(file, joke[i]);
        i++;
    }
    if (ch == 35)
        ch = 0;
   if (!joke[ch].empty() && joke[ch][0] == '\n' && joke[ch].length() > 1)
        joke[ch].erase(0, 1);
    write_to_client(user.clientFd, RPL_MOTD(user.nickName, joke[ch]));
    ch++;
}
