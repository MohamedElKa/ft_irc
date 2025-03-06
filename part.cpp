#include "command.hpp"
#include "ft_irc.hpp"

void    BroadPart(std::string &reason, std::vector<Channel>::iterator it, Client &user)
{
    for (size_t i = 0; i < it->userlist.size(); ++i)
    {
        write_to_client(it->userlist[i].clientFd, PART_MSG(user.nickName, user.userName, user.ipaddr, it->getname(), reason));
    }
}

void    part(Command &cmd, std::vector<Channel>&channels, Client &user)  //Parameters: <channel>{,<channel>} [<reason>]
{
    if (cmd.params.size() == 0)
        return write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command));
    for (size_t i = 0; i < cmd.channels_name.size(); ++i)
    {
        if (cmd.channels_name.at(i)[0] != '#')
            write_to_client(user.clientFd, ERR_BADCHANMASK(cmd.channels_name.at(i)));
        else
        {
            std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), Channel(cmd.channels_name.at(i)));
            if (it != channels.end())
            {
                if (it->is_user(user))
                {
                    BroadPart(cmd.reason, it, user);
                    if (it->is_operator(user))
                        it->removeop(Client(user));
                    it->removeuser(Client(user));
                    if (it->userlist.size() == 0)
                        channels.erase(it);
                }
                else
                    write_to_client(user.clientFd, ERR_NOTONECHANNEL(user.nickName, it->getname()));
            }
            else
                write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(i)));
        }
    }
}
