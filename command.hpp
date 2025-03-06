# ifndef COMMAND_HPP
#define COMMAND_HPP

#include "channel.hpp"
#include <unistd.h>
#include "ft_irc.hpp"
#include <iomanip>
#include <cstring>
#include <ctime>
#include <utility>
#include <sstream>
#include <string>
#include <vector>

#define NBCOMMANDS 12

class Command
{
public:
    std::string bot_arg;
    std::string command;
    std::string rawline;
    std::string topic;
    std::string reason;
    std::string toinvite;
    std::string mode_target;
    std::string message;
    std::string __nick;
    bool        bad_command;
    bool        not_command;

    std::vector < std::pair<char, char> > mode_string;
    std::vector<std::string>users;
    std::vector<std::string>channels_name;
    std::vector<std::string>passwords;
    std::vector<std::string>params;
    std::vector<std::string>mode_params;

    Command();

    void    valid(std::string &first);
    void    parse(const char *str);
    void    parse_commands(void);
	void    parseJoin(void);
	void	parseTopic(void);
	void	parseKick(void);
	void	parseQuit(void);
    void    parseNick(void);
    void    parseInvite(void);
    void    parseMode(void);
    void    parsePart(void);
    void    parsePRIVMSG(void);
};

void    write_to_client(int __fd, const std::string &__msg);
void    join(Command &, std::vector<Channel>&, Client &);
void    topic(Command &, std::vector<Channel>&, Client &);
void    invite(Command &, std::vector<Channel>&, Client &, std::vector<Client> &);
void    kick(Command &, std::vector<Channel>&, Client &);
void    mode(Command &, std::vector<Channel>&, Client &);
void    quit(Command& cmd, Client &user, std::vector<Channel> &channels);
void    part(Command &, std::vector<Channel>&, Client &);
void    nick(Command &, Client &, std::vector<Channel> &, std::vector<Client> &);
void    privateMsg(Command&, Client &, std::vector<Channel> &,std::vector<Client> &);
void    notice(Command&, Client &, std::vector<Channel> &,std::vector<Client> &);
void    bot(Client &user);

#endif