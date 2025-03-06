#include "channel.hpp"
#include "Client.hpp"
#include "command.hpp"
#include "ft_irc.hpp"

void    BroadKick(std::vector<Channel>::iterator it, Client &user, std::string &tokick, std::string &reason)
{
    for (size_t i = 0; i < it->userlist.size(); ++i)
    {
        write_to_client(it->userlist[i].clientFd, KICK_MSG(user.nickName, user.ipaddr, it->getname(), tokick, reason));
    }
}

void    kick(Command &cmd, std::vector<Channel>&channels, Client &user)
{
    if (cmd.params.size() > 1)
    {
        if (cmd.channels_name.at(0)[0] != '#')
           return  write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(0)));
        Channel temp(cmd.channels_name.at(0));
        std::vector<Channel>::iterator iter = std::find(channels.begin(), channels.end(), temp);
        if (iter != channels.end())
        {
            if (iter->is_operator(user))
            {
                for (size_t i = 0; i < cmd.users.size(); ++i)
                {    
                    if (iter->is_user(Client(cmd.users.at(i))) == true)
                    {
                        BroadKick(iter, user, cmd.users.at(i), cmd.reason);
                        if (iter->is_operator(Client(cmd.users.at(i))))
                            iter->removeop(cmd.users.at(i));
                        iter->removeuser(Client(cmd.users.at(i)));
                        if (iter->userlist.size() == 0)
                            channels.erase(iter);
                    }
                    else
                        write_to_client(user.clientFd, ERR_NOSUCHNICK(user.nickName, cmd.users.at(i)));
                }
            }
            else
                write_to_client(user.clientFd, ERR_CHANOPRIVSNEEDED(user.nickName, iter->getname()));
        }
        else
            write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(0)));
    }
    else
        write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command));
}