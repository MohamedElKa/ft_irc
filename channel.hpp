#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Client.hpp"


class Channel
{
private:
    std::string name;
    // std::string password;
    // bool        pass_r;
public:
    time_t  topic_date;
    time_t  creation_time;
    std::string topic;
    std::string topicsetter;
    std::string mode_changes;
    std::string oldmode;
    std::string modestring;

    ssize_t number_users;
    char  i_mode;
    char  t_mode;
    std::pair<char, ssize_t> l_mode;
    std::pair<char, std::string> k_mode;
    std::pair<std::string, std::string> o_plusmode;
    std::pair<std::string, std::string> o_minusmode;
    std::pair<char, ssize_t> old_lmode;

    std::vector<Client>userlist;
    std::vector<Client>oplist;
    std::vector<Client>invitelist;

    Channel(const std::string &);
    bool    operator==(const Channel &) const;

    void    getmodestring();
    void    getmode_changes();
    void    adduser(const Client &);
    void    addop(const Client &);
    void    addinvite(const Client &);

    void    removeuser(const Client &);
    void    removeop(const Client &);
    void    removeinvite(const Client &);

    bool    is_operator(const Client &);
    bool    is_user(const Client &);
    bool    is_invited(const Client &);
    // bool    pass_required () const;

    std::string getname() const;
    std::string getopic() const;
    // std::string getpass() const;
};
std::string toString(int n);
std::vector<Channel>::iterator check_channels(std::vector<Channel>&, const std::string &);

#endif