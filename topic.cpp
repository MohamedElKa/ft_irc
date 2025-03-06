#include "Client.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <vector>

bool   view_topic(Command &cmd, std::vector<Channel>::iterator it, Client &user)
{
    if (cmd.topic.empty())
    {
        if (it->getopic().empty())
            write_to_client(user.clientFd, RPL_NOTOPIC(user.nickName, it->getname()));
        else
        {
            write_to_client(user.clientFd, RPL_TOPIC(user.nickName, it->getname(), it->getopic()));
            write_to_client(user.clientFd, RPL_TOPICWHOTIME(user.nickName, it->getname(), it->topicsetter, toString(it->topic_date)));
        }
        return true;
    }
    return false;
}

void    BroadTopic(std::vector<Channel>::iterator it, Command &cmd, Client &user)
{
    for (size_t i = 0; i < it->userlist.size(); ++i)
    {
        write_to_client(it->userlist[i].clientFd, RPL_TOPIC(user.nickName, it->getname(), cmd.topic));
    }
}

void    topic(Command &cmd, std::vector<Channel>&channels, Client &user)
{
    if (cmd.params.empty())
    {
       write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command));
       return ;
    }
    std::vector<Channel>::iterator it = check_channels(channels, cmd.channels_name.at(0)); // check if channel exist in server
    if (it != channels.end())
    {
        if (view_topic(cmd, it, user) == true)
            return ;
        if (!it->is_user(user))
            return  write_to_client(user.clientFd, ERR_NOTONECHANNEL(user.nickName, it->getname()));
        if (it->t_mode == '+')
        {
            if (!it->is_operator(user))
                return write_to_client(user.clientFd, ERR_CHANOPRIVSNEEDED(user.nickName, it->getname()));
        }
        it->topic_date = time(NULL);
        if (cmd.topic.length() > 60)
            it->topic = cmd.topic.substr(0, 60);
        else
            it->topic = cmd.topic;
		it->topicsetter = user.nickName;
        BroadTopic(it, cmd, user);
    }
    else
        write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(0)));
}