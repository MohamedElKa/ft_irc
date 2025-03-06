#include "channel.hpp"
#include "Client.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <utility>

bool	is_mode_char(char c)
{
	if ( c == 'i' || c == 'o' 
	  || c == 'l' || c == 't'
	  || c == 'k')
		return true;
	return false;
}

void	BroadMode(std::vector<Channel>::iterator it, Client &user, std::string &added_mode)
{
	for (size_t i = 0; i < it->userlist.size(); ++i)
	{
		write_to_client(it->userlist[i].clientFd, MODE_MSG(user.nickName, user.userName, user.ipaddr, it->getname(), added_mode, ""));
	}
}

void    mode(Command &cmd, std::vector<Channel> &channel, Client &user)
{
    if (cmd.params.size() == 0)
       return write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, cmd.command));
	std::vector<Channel>::iterator it = std::find(channel.begin(), channel.end(), Channel(cmd.channels_name.at(0)));
	if (it != channel.end())
	{
		it->oldmode = it->modestring;
		if (cmd.mode_string.empty()) {
			if (it->is_user(user)) {
				write_to_client(user.clientFd, RPL_CHANNELMODEIS(user.nickName, it->getname(), it->modestring));
				write_to_client(user.clientFd, RPL_CREATIONTIME(user.nickName, it->getname(), toString(it->creation_time)));}
			else
				write_to_client(user.clientFd, ERR_NOTONECHANNEL(user.nickName, it->getname()));
			return ;
		}
		if (it->is_operator(user))
		{
			size_t i = 0;
			size_t param_i = 0;
			while (i < cmd.mode_string.size())
			{
				char c = cmd.mode_string.at(i).second;
				if (c != 'n' && c != 's')
				{
					if (!is_mode_char(c))
						write_to_client(user.clientFd, ERR_UNKNOWNMODE(user.nickName, c));
					else
					{
						std::string op = "+";
						if (cmd.mode_string.at(i).first != 0)
							op = cmd.mode_string.at(i).first;
						if (c == 'i')
                    		it->i_mode = op[0];
						else if (c == 't')
                    		it->t_mode = op[0];
						else if (c == 'l')
						{
							if (op == "-")
							{
								it->l_mode.first = '-';
								it->l_mode.second = -1;
							}
							else
							{
								if (param_i < cmd.mode_params.size())
								{
									it->old_lmode = it->l_mode;
									it->l_mode.first = '+';
									int value = std::atoi(cmd.mode_params.at(param_i).c_str());
									if (value > 0)
										it->l_mode.second = (ssize_t)value;
								}
								else
									write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, " MODE " + op + c));
								param_i += 1;
							}
                		}
						else if (c == 'k')
						{
							char temp = it->k_mode.first;
                    		std::string key = "";
							if (param_i < cmd.mode_params.size())
							{
								key = cmd.mode_params.at(param_i);
								if (temp  == op[0])
									write_to_client(user.clientFd, ERR_KEYALREADYSET(user.nickName, it->getname()));
								if (it->k_mode.first != temp && it->k_mode.second != key && !it->k_mode.second.empty())
									write_to_client(user.clientFd, ERR_KEYALREADYSET(user.nickName, it->getname()));
								else if (temp  != op[0])
								{
									if (op == "+")
									{
										it->k_mode.first = '+';
										it->k_mode.second = key;
									}
									else
									{
										if (it->k_mode.second == key)
										{
											it->k_mode.first = '-';
											it->k_mode.second = "";
										}
										else
											write_to_client(user.clientFd, ERR_KEYALREADYSET(user.nickName, it->getname()));
									}
								}
							}
							else
								write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, " MODE " + op + c));
							param_i += 1;
						}
				 		else if (c == 'o'){
							if (param_i < cmd.mode_params.size())
							{
								if (it->is_user(Client(cmd.mode_params.at(param_i))))
								{
									if (op[0] == '+' && !it->is_operator(Client(cmd.mode_params.at(param_i))))
									{
										it->addop(Client(cmd.mode_params.at(param_i)));
										it->o_plusmode.first += 'o';
										it->o_plusmode.second += cmd.mode_params.at(param_i) + " ";
									}
									if (op[0] == '-' && it->is_operator(Client(cmd.mode_params.at(param_i))))
									{
										it->removeop(Client(cmd.mode_params.at(param_i)));
										it->o_minusmode.first += 'o';
										it->o_minusmode.second += cmd.mode_params.at(param_i) + " ";
									}
								}
								else
									write_to_client(user.clientFd, ERR_NOSUCHNICK(user.nickName, cmd.mode_params.at(param_i)));
							}
							else
								write_to_client(user.clientFd, ERR_NEEDMOREPARAMS(user.nickName, " MODE " + op + c));
							param_i += 1;
						}
					}
				}
				++i;
			}
			it->getmodestring();
			if (!it->mode_changes.empty())
				BroadMode(it, user, it->mode_changes);
			it->o_minusmode = std::make_pair("", ""); it->o_plusmode = std::make_pair("", "");;
		}
		else
			return write_to_client(user.clientFd, ERR_CHANOPRIVSNEEDED(user.nickName, it->getname()));
	}
	else
		write_to_client(user.clientFd, ERR_NOSUCHCHANNEL(user.nickName, cmd.channels_name.at(0)));
}