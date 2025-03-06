#include "command.hpp"
#include <cstddef>
#include <string>
#include <variant>

Command::Command()
{
	__nick = "";
    command = "";
	topic = "";
	bad_command = false;
	not_command = false;
	toinvite = "";
	message = "";
	bot_arg = "";
	reason = "";
}

void	Command::valid(std::string &f)
{
    std::string s;
    for (size_t i = 0; i < f.length(); i++)
       s += std::tolower(f[i]);
	if (s != "join" && s != "topic" 
		&& s != "kick" && s != "bot"
		&& s != "mode" && s != "quit" && s != "notice"
		&& s != "invite" && s != "part" && s != "privmsg"
		&& s != "pong" && s!= "nick" && s != "user")
		bad_command = true;
	command = s;
}

void	Command::parseJoin()
{
	if (params.size() > 0)
	{
		std::stringstream split(params.at(0));
    	std::string token;

    	while (std::getline(split, token, ','))
    	    channels_name.push_back(token);
		if (params.size() > 1)
		{
			std::stringstream split2(params.at(1));
			while (std::getline(split2, token, ','))
				passwords.push_back(token);
		}
	}
}

void	Command::parseTopic() // :asdasdasd sadas asdasd as asd
{   
	if (params.size() > 0)
	{
		channels_name.push_back(params.at(0));
		if (params.size() > 1)
		{
			for (size_t i = 1; i < params.size(); ++i)
			{
				if (params.at(i)[0] == ':')
				{
					size_t pos = rawline.find(params.at(i)) + 1;
					if (rawline[pos] == '\n')
						topic = "";
					else
					{
						rawline = rawline.substr(pos, rawline.length() - pos);
						pos = rawline.find_last_not_of("\n");
						if (pos != std::string::npos && pos < rawline.length())
							rawline.erase(pos + 1);
						topic = rawline;
					}
					return ;
				}
			}
			topic = params.at(params.size() - 1);
		}
	}
}

void	Command::parsePRIVMSG()
{
	if (params.size() > 0)
	{
		std::stringstream split(params.at(0));
    	std::string token;
    	while (std::getline(split, token, ','))
				users.push_back(token);
		if (params.size() > 1)
		{
			for (size_t i = 1; i < params.size(); ++i) //privmsg targets,...,targets   : the whole message.....!
			{
				if (params[i][0] == ':')
				{
					size_t pos = rawline.find(params.at(i)) + 1;
					if (rawline[pos] == '\n')
						message = "";
					else
					{
						rawline = rawline.substr(pos, rawline.length() - pos);
						pos = rawline.find_last_not_of("\n");
						if (pos != std::string::npos && pos < rawline.length())
							rawline.erase(pos + 1);
						message = rawline;
					}
					return ;
				}
			}
			message = params.at(params.size() - 1);
		}
	}
}

void	Command::parseKick() //  : reason ... (text)
{
	if (params.size() > 0)
	{
		channels_name.push_back(params.at(0));
		if (params.size() > 1)
		{
			std::stringstream split(params.at(1));
    		std::string token;
    		while (std::getline(split, token, ','))
				users.push_back(token);
		}
		if (params.size() > 2)
		{
			for (size_t i = 2; i < params.size(); ++i)
			{
				if (params.at(i)[0] == ':')
				{	
					size_t pos = rawline.find(params.at(i));
					if (rawline[pos] == '\n')
						reason = "";
					else
					{
						rawline = rawline.substr(pos, rawline.length() - pos);
						pos = rawline.find_last_not_of("\n");
						if (pos != std::string::npos && pos < rawline.length())
							rawline.erase(pos + 1);
						reason = rawline;
					}
					return ;
				}
			}
			reason = ":" + params.at(params.size() - 1);
		}
	}
}

void	Command::parseNick(void)
{
	if (params.size() > 0)
	{
		if (params[0][0] == ':')
			__nick = params[0].substr(1);
		else
			__nick = params.at(0);
	}
}

void	Command::parseInvite(void)
{
	if (params.size() > 0)
		toinvite = params.at(0);
	if (params.size() > 1)
		channels_name.push_back(params.at(1));
}

void	Command::parseMode(void)
{
	if (params.size() > 0)
		channels_name.push_back(params.at(0));
	if (params.size() > 1)
	{
		char op = '+';   //default operator '+' mode #channel
		for (size_t i = 0; i < params.at(1).length(); ++i)
		{
			if (params.at(1).at(i) == '+' || params.at(1).at(i) == '-') // oi-lk
				op = params.at(1).at(i);
			else
			{
				mode_string.push_back(std::make_pair(op, params.at(1).at(i)));// <+,o>, <+,i>,<-,l>,<>
			}
		}
	}
	if (params.size() > 2)
	{
		for (size_t i = 2; i < params.size(); ++i)
			mode_params.push_back(params.at(i));
	}
}

void	Command::parseQuit()
{
	for (size_t i = 0; i < params.size(); ++i)
	{
		if (params.at(i)[0] == ':')
		{	
			size_t pos = rawline.find(params.at(i)) + 1;
			if (rawline[pos] == '\n')
				reason = "";
			else
			{
				rawline = rawline.substr(pos, rawline.length() - pos);
				pos = rawline.find_last_not_of("\n");
				if (pos != std::string::npos && pos < rawline.length())
					rawline.erase(pos + 1);
				reason = rawline;
			}
			return ;
		}
	}
	if (params.size() > 0)
		reason = params.at(params.size() - 1);
}

void	Command::parsePart()
{
	if (params.size() > 0)
	{
		std::stringstream split(params.at(0));
    	std::string token;

    	while (std::getline(split, token, ','))
    	    channels_name.push_back(token);
		if (params.size() > 1)
		{
			for (size_t i = 1; i < params.size(); ++i)
			{
				if (params[i][0] == ':')
				{
					size_t pos = rawline.find(params.at(i));
					if (rawline[pos] == '\n')
						reason = "";
					else
					{
						rawline = rawline.substr(pos, rawline.length() - pos);
						pos = rawline.find_last_not_of("\n");
						if (pos != std::string::npos && pos < rawline.length())
							rawline.erase(pos + 1);
						reason = rawline;
					}
					return ;
				}
			}
			reason = ":" + params[params.size() - 1];
		}
	}
}

void	Command::parse_commands()
{
	if (bad_command == true)
		return ;
	std::string cmds[] = {"join", "topic", "kick", "invite", "mode", "quit", "part", "privmsg", "nick", "notice"};
	void (Command::*ptr[])(void) = {&Command::parseJoin, &Command::parseTopic, &Command::parseKick,&Command::parseInvite,\
	&Command::parseMode, &Command::parseQuit,&Command::parsePart, &Command::parsePRIVMSG, &Command::parseNick, &Command::parsePRIVMSG};
	for (size_t i = 0; i < NBCOMMANDS; ++i)
	{
		if (command == cmds[i])
		{
			(this->*ptr[i])();
			return ;
		}
	}
}

void    Command::parse(const char *str)
{
	rawline = str;
    std::istringstream parse(rawline);
    std::string first;
    std::string param;

    parse >> first;
    while (parse >> param)
	{
        params.push_back(param);
	}
	if (first[0] == '/')
		first = first.substr((size_t)1);
	valid(first);
	parse_commands();
}