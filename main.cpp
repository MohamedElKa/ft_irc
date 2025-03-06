/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-karm <mel-karm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:12:06 by mel-karm          #+#    #+#             */
/*   Updated: 2023/12/31 14:46:40 by mel-karm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "usage:\n";
        std::cerr << "./ircserver [port] [password]\n";
        return 1;
    }
    if (!is_digit(av[1]))
    {
        std::cerr << "invalid port char\n";
        return 1;
    }
    int port = atoi(av[1]);
    if (!(port >= 2000 && port <= 9000))
    {
        std::cerr << "Valid port in this server is ranged (2000 to 9000)\n";
        return 1;
    }
    Launch(av[1], av[2]);
    return 0;
}