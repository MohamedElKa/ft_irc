#include "Client.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <algorithm>
#include <vector>

void    invite(Command &cmd, std::vector<Channel>&channels, Client &user, std::vector<Client> &clients)
{
    std::vector<Channel>::iterator it;
    if (cmd.params.size() == 0)
        return  write_to_client(user.clientFd, RPL_ENDOFINVITELIST(user.nickName));
    if (cmd.params.size() == 1)
        return  write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command));
    it = std::find(channels.begin(), channels.end(), Channel(cmd.channels_name.at(0)));
    if (it != channels.end())
    {
        if (it->is_operator(user))
        {
            std::vector<Client>::iterator iter = std::find(clients.begin(), clients.end(), Client(cmd.toinvite));
            if (iter != clients.end())
            {
                if (it->is_user(Client(cmd.toinvite)))
                    return write_to_client(user.clientFd, ERR_USERONCHANNEL(user.nickName, cmd.toinvite, it->getname()));
                it->invitelist.push_back(Client(cmd.toinvite));
                write_to_client(user.clientFd, RPL_INVITING(user.nickName, cmd.toinvite, it->getname()));
                write_to_client(iter->clientFd, RPL_INVITING(user.nickName, cmd.toinvite, it->getname()));
            }
            else
                return write_to_client(user.clientFd, ERR_NOSUCHNICK(user.nickName, cmd.toinvite));
        }
        else
            write_to_client(user.clientFd, ERR_CHANOPRIVSNEEDED(user.nickName, it->getname()));
    }
    else
        write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(0)));
}