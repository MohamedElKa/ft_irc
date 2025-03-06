#include "command.hpp"
#include "ft_irc.hpp"

std::vector<Channel>::iterator check_channels(std::vector<Channel>& channels, const std::string &channel_name)
{
    if (!channels.empty())
    {
        std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), Channel(channel_name));
        if (it != channels.end())
            return it;
    }
    return channels.end();
}

void    BroadJoin(std::vector<Channel>::iterator it, Client &user)
{
    for (size_t i = 0; i < it->userlist.size(); ++i)
    {
       // if (it->userlist[i].nickName != user.nickName)
            write_to_client(it->userlist[i].clientFd, JOIN_SUCC(user.nickName, user.userName, user.ipaddr, it->getname()));
    }
}

std::string    get_users_list(Channel &_channle)
{
    std::string list = "";
    for (size_t i = 0; i < _channle.userlist.size(); ++i)
    {
        if (_channle.is_operator(_channle.userlist.at(i)))
            list += "@";
        list += _channle.userlist.at(i).nickName;
        if (i + 1 < _channle.userlist.size())
            list += " ";
    }
    return list;
}

void    join(Command &cmd, std::vector<Channel>&channels, Client &user)
{
    if (cmd.params.size() == 0)
        return write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command)); // if join has no parameters
    for (size_t i = 0; i < cmd.channels_name.size(); ++i)
    {
        if (channels.size() >= 20)
                return write_to_client(user.clientFd, ERR_TOOMANYCHANNELS(user.nickName, cmd.channels_name[i]));
        if (cmd.channels_name.at(i)[0] == '#')
        {
            std::vector<Channel>::iterator it;
            it = check_channels(channels, cmd.channels_name.at(i));
            if (it == channels.end())
            {
                channels.push_back(Channel(cmd.channels_name.at(i)));
                channels.at(channels.size() - 1).adduser(user);
                channels.at(channels.size() - 1).addop(user);
                write_to_client(user.clientFd, JOIN_SUCC(user.nickName, user.userName, user.ipaddr, cmd.channels_name.at(i)));
                write_to_client(user.clientFd, RPL_NAMERPLY(user.nickName, cmd.channels_name.at(i), get_users_list(channels.at(channels.size() - 1))));
                write_to_client(user.clientFd, RPL_ENDOFNAMES(user.nickName, cmd.channels_name.at(i)));
            }
            else
            {
                if (it->l_mode.first == '+' && it->l_mode.second > 0)
                {
                    if (it->l_mode.second > 51)
                        it->l_mode.second = 51;
                    if ((size_t)it->l_mode.second == it->userlist.size())
                        return write_to_client(user.clientFd, ERR_CHANNELISFULL(user.nickName, it->getname()));
                }
				if (it->is_user(user) == false)
				{
                    if (it->i_mode == '+' && !it->is_invited(user))
                        return write_to_client(user.clientFd, ERR_INVITEONLYCHAN(user.nickName, it->getname()));
                	if (it->k_mode.first == '+')
                	{
                    	if (cmd.passwords.size() > 0)
                    	{
                        	if (cmd.passwords.at(i) == it->k_mode.second)
                        	{
                            	it->adduser(user);
                                BroadJoin(it, user);
                                write_to_client(user.clientFd, RPL_NAMERPLY(user.nickName, cmd.channels_name.at(i), get_users_list(*it)));
                                write_to_client(user.clientFd, RPL_ENDOFNAMES(user.nickName, it->getname()));
                        	}
                        	else
                            	write_to_client(user.clientFd, ERR_BADCHANNELKEY(user.nickName, it->getname()));
                    	}
                        else
                            write_to_client(user.clientFd, ERR_BADCHANNELKEY(user.nickName, it->getname()));
                	}
                	else
                	{
                    	it->adduser(user);
                    	BroadJoin(it, user);
                        write_to_client(user.clientFd, RPL_NAMERPLY(user.nickName, cmd.channels_name.at(i), get_users_list(*it)));
                        write_to_client(user.clientFd, RPL_ENDOFNAMES(user.nickName, it->getname()));
                	}
				}
            }
        }
        else
             write_to_client(user.clientFd, ERR_BADCHANMASK(cmd.channels_name.at(i)));
    }
}