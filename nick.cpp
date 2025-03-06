#include "command.hpp"
#include "ft_irc.hpp"

void BroadNick(std::vector<Client> &clientlist, Client &user, std::string &old, std::vector<Client>&sm)
{
    for (size_t i = 0; i < clientlist.size(); ++i)
    {
        if (clientlist[i].clientFd != user.clientFd)
        {
            if (std::find(sm.begin(), sm.end(), clientlist[i]) == sm.end())
            {
                write_to_client(clientlist[i].clientFd, NICK_MSG(old, user.userName, user.ipaddr, user.nickName));
                sm.push_back(clientlist[i]);
            }
        }
    }
}

void    change_nick_channels(Client &user, std::vector<Channel> &ch, std::string &newNick)
{
    for (size_t i = 0; i < ch.size(); ++i)
    {
        std::vector<Client>::iterator it = std::find(ch[i].userlist.begin(), ch[i].userlist.end(), user);
        if (it != ch[i].userlist.end())
        {
            if (newNick.length() > 15)
                it->nickName = newNick.substr(0, 15);
            else
                it->nickName = newNick;
        }
    }
}

void nick(Command &cmd, Client &user, std::vector<Channel> &channels, std::vector<Client> &clients)
{
    if (cmd.params.size() == 0 || cmd.__nick.empty())
        return write_to_client(user.clientFd, ERR_NOGIVENNICK(user.nickName));
    if (std::isdigit(cmd.__nick[0]) || cmd.__nick[0] == '#')
        return write_to_client(user.clientFd, ERR_ONEOUS(user.nickName, cmd.__nick));
    std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), Client(cmd.__nick));
    if (it == clients.end())
    {
        std::string oldnick = user.nickName;
        std::vector<Client>sendmsg;
        change_nick_channels(user, channels, cmd.__nick);
        if (cmd.__nick.length() > 15)
            user.nickName = cmd.__nick.substr(0, 15);
        else
            user.nickName = cmd.__nick;
        write_to_client(user.clientFd, NICK_MSG(oldnick, user.userName, user.ipaddr, user.nickName));  
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (channels[i].is_user(user))
                BroadNick(channels[i].userlist, user, oldnick, sendmsg);
        }
    }
    else
        write_to_client(user.clientFd, ERR_NICKINUSE(user.nickName, cmd.__nick));
}
