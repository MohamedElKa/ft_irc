#include "Client.hpp"
#include "channel.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <vector>

void    notice(Command& cmd, Client &user, std::vector<Channel> &channels,std::vector<Client> &clients)
{
    if (cmd.params.size() > 0)
    {
		if (cmd.message.empty())
			return write_to_client(user.clientFd, ERR_NOTEXTTOSEND(user.nickName));
		size_t i = 0;
    	while (i < cmd.users.size())
    	{
			if (cmd.users.at(i)[0] == '#')
			{
				std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), Channel(cmd.users.at(i)));
				if (it != channels.end())
				{
					for (size_t i = 0; i < it->userlist.size(); ++i)
					{
						if (it->userlist.at(i).clientFd != user.clientFd)
							write_to_client(it->userlist.at(i).clientFd, NOTICE_MSG(user.nickName, user.userName, user.ipaddr, it->getname(), cmd.message));	
					}
				}
				else
					write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.users.at(i)));
			}
			else // user
			{
				std::vector<Client>::iterator it = std::find_if(clients.begin(), clients.end(), Client(cmd.users.at(i)));
				if (it != clients.end())
					write_to_client(it->clientFd, NOTICE_MSG(user.nickName, user.userName, user.ipaddr, it->nickName, cmd.message));
				else
					write_to_client(user.clientFd, ERR_NOSUCHNICK(user.nickName, cmd.users.at(i)));
			}
			++i;
    	}
    }
    else 
        write_to_client(user.clientFd, ERR_NORECIPENT(user.nickName, "(NOTICE)"));
}