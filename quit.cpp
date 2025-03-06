#include "channel.hpp"
#include "command.hpp"
#include "ft_irc.hpp"
#include <cstddef>
#include <vector>

void    BroadQuit(std::vector<Client> &clientlist, Client &user, std::string &reason, std::vector<Client>&sentusers)
{
    for (size_t i = 0; i < clientlist.size(); ++i)
    {
        if (std::find(sentusers.begin(), sentusers.end(), Client(clientlist[i])) == sentusers.end())
        {
            write_to_client(clientlist[i].clientFd, QUIT_MSG(user.nickName, user.userName, user.ipaddr, reason));
            sentusers.push_back(clientlist[i]);
        }
    }
}

void quit(Command& cmd, Client &user, std::vector<Channel> &channels)
{
    close(user.clientFd);
    if (!cmd.reason.empty())
        cmd.reason = ": " + cmd.reason;
    std::vector<Client> sentusers;
    std::vector<Channel>::iterator iter = channels.begin();
    while (iter != channels.end())
    {
        if (iter->is_user(user))
        {
            iter->removeuser(user);
            iter->removeop(user);
            iter->removeinvite(user);
            BroadQuit(iter->userlist, user, cmd.reason, sentusers);
            if (iter->userlist.empty())
                iter = channels.erase(iter); // Update the iterator after erasing
            else 
                ++iter;
        }
        else
            ++iter;
    }
}

