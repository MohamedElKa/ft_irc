#include "Client.hpp"
#include "command.hpp"
#include "server.hpp"
#include "ft_irc.hpp"
#include <vector>

void    Server::execute(Command &cmd, Client &user)
{
    if (cmd.command.empty())
        return ;
    if (cmd.bad_command == true)
    {
        write_to_client(user.clientFd, ERR_COMMANDNOTFOUND(user.nickName, cmd.command));
        return ;
    }
    int to_execute = -1;
    std::string	validcmds[NBCOMMANDS] = {"join", "topic", "kick",\
     "invite", "mode", "quit", "part", "privmsg", "nick", "bot", "notice"};
    for (int i = 0; i < NBCOMMANDS; ++i)
        if (cmd.command == validcmds[i]) {
            to_execute = i;
            break ;
        }
    switch (to_execute)
    {
        case 0: join(cmd, channels, user);  break ;
        case 1: topic(cmd, channels, user); break ;
        case 2: kick(cmd, channels, user) ; break ;
        case 3: invite(cmd, channels, user, clientsList); break ;
        case 4: mode(cmd, channels, user) ; break ;
        case 5: quit(cmd, user, channels); break ;
        case 6: part(cmd, channels, user); break ;
        case 7: privateMsg(cmd, user, channels,clientsList); break ;
        case 8: nick(cmd, user, channels, clientsList); break ;
        case 9: bot(user); break ;
        case 10: notice(cmd, user, channels,clientsList); break;
        default:
            break ;
    }
}