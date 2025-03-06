/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Launch.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:29:48 by mel-karm          #+#    #+#             */
/*   Updated: 2024/01/06 13:55:19 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "server.hpp"
#include <cstdlib>
#include <netdb.h>


void Launch(std::string port, std::string password)
{
    Server serverObject(port, password);
    serverObject.createSocket();
    serverObject.bindingSocket();
    serverObject.socketOnListening();
    serverObject.acceptingClient();
    freeaddrinfo(serverObject.p);
    close(serverObject.serverFd);
}