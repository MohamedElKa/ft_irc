#include "channel.hpp"
#include <sys/fcntl.h>
 
void    Channel::getmode_changes()
{
    mode_changes = "";
    size_t i = 0;
    bool    signe = false;
    while (i < oldmode.length())
    {
         if (i == 4)
            break ;
        if (!std::isdigit(oldmode[i]))
        if (modestring.find(oldmode[i]) == std::string::npos)
        {
            if (signe == false)
                mode_changes += "-";
            mode_changes += oldmode[i];
            signe = true;
        }
        ++i;
    }
    if (!o_minusmode.second.empty() && !o_minusmode.first.empty())
    {
        i = mode_changes.find('-');
        if (i == std::string::npos)
            mode_changes += "-";
        mode_changes += o_minusmode.first;
    }
    i = 0;
    signe = false;
    while (i < modestring.length())
    {
        if (i == 4)
            break ;
        if (!std::isdigit(modestring[i]))
        if (oldmode.find(modestring[i]) == std::string::npos)
        {
            if (signe == false)
                mode_changes += "+";
            mode_changes += modestring[i];
            signe = true;
        }
        ++i;
    }
    if (oldmode.find('l') != std::string::npos && old_lmode != l_mode && l_mode.first == '+')
    {
        i = mode_changes.find('+');
        if (i == std::string::npos)
            mode_changes += "+";
        mode_changes += 'l';
    }
    if (!o_plusmode.second.empty() && !o_plusmode.first.empty())
    {
        i = mode_changes.find('+');
        if (i == std::string::npos)
            mode_changes += "+";
        mode_changes += o_plusmode.first;
    }
    if (mode_changes.find('k') != std::string::npos && k_mode.first != '-')
    {
        mode_changes += " ";
        if (k_mode.second.empty() == false)
            mode_changes += k_mode.second;
    }
    if (mode_changes.find('l') != std::string::npos)
    {
        mode_changes += " ";
        if (l_mode.second != -1)
            mode_changes += toString(l_mode.second);
    }
    std::string names;
    std::istringstream snames(o_minusmode.second);
    while (snames >> names)
        mode_changes += " " + names;;
    std::istringstream ssnames(o_plusmode.second);
    while (ssnames >> names)
        mode_changes += " " + names;
}

void Channel::getmodestring()
{
    modestring = "";
    if (i_mode == '+')
        modestring += "i";
    if (t_mode == '+')
       modestring += "t";
    if (k_mode.first == '+' && !k_mode.second.empty())
        modestring += "k";
    if (l_mode.first == '+' && l_mode.second != -1)
        modestring += "l";
    if (!k_mode.second.empty())
    {
        modestring += " ";
        modestring += k_mode.second;
    }
    if (l_mode.second != -1)
    {
        modestring += " ";
        std::ostringstream conv;
        if (l_mode.second > 51)
            l_mode.second = 51;
        conv << l_mode.second;
        modestring += conv.str();
    }
    if (!oldmode.empty())
        oldmode.erase(0, 1);
    getmode_changes();
    if (!modestring.empty())
        modestring.insert(0, 1, '+');
}

Channel::Channel(const std::string &_name)
{
    creation_time = time(NULL);
    topic_date = time(NULL);
    name = _name;
    mode_changes = "";
    modestring = "";
    topic = "";
    number_users = -1;
    topicsetter = "";
    oldmode = "";
    i_mode = '-';
    t_mode = '+';
    k_mode = std::make_pair('-', "");
    l_mode = std::make_pair('-', number_users);
    o_minusmode = std::make_pair("", "");
    o_plusmode = std::make_pair("", "");
    getmodestring();
}

bool    Channel::operator==(const Channel &ch) const
{
    if (this->name == ch.getname())
        return true;
    return false;
}

std::string Channel::getname() const
{
    return this->name;
}

std::string Channel::getopic() const
{   
    return this->topic;
}

void    Channel::adduser(const Client &user)
{
    std::vector<Client>::iterator it = std::find(userlist.begin(), userlist.end(), user);
    if (it == userlist.end())
    {
        userlist.push_back(user);
    }
}

void    Channel::addop(const Client &op)
{
    if (oplist.size() == 0)
    {
        oplist.push_back(op);
        return ;
    }
    std::vector<Client>::iterator it = std::find(oplist.begin(), oplist.end(), op);
    if (it == oplist.end())
        oplist.push_back(op);
}

void    Channel::removeuser(const Client &user)
{
    if (userlist.size() == 0)
        return ;
    std::vector<Client>::iterator it = std::find(userlist.begin(), userlist.end(), user);
    if (it != userlist.end())
    {
        userlist.erase(it);
    }
}

void    Channel::removeop(const Client &op)
{
    if (oplist.size() == 0)
        return ;
    std::vector<Client>::iterator it = std::find(oplist.begin(), oplist.end(), op);
    if (it != oplist.end())
    {
        oplist.erase(it);
    }
}
void    Channel::addinvite(const Client &user)
{
    if (invitelist.size() == 0)
    {
        invitelist.push_back(user);
        return ;
    }
   std::vector<Client>::iterator it = std::find(invitelist.begin(), invitelist.end(), user);
    if (it == invitelist.end())
        invitelist.push_back(user); 
}

void    Channel::removeinvite(const Client &user)
{
    if (invitelist.size() == 0)
        return ;
    std::vector<Client>::iterator it = std::find(invitelist.begin(), invitelist.end(), user);
    if (it != invitelist.end())
        invitelist.erase(it); 
}

bool    Channel::is_operator(const Client &client)
{
    if (oplist.size() == 0)
        return false;
    std::vector<Client>::iterator it = std::find(oplist.begin(), oplist.end(), client);
    if (it != oplist.end())
        return true;
    return false;
}

bool    Channel::is_user(const Client &client)
{
    std::vector<Client>::iterator it = std::find(userlist.begin(), userlist.end(), client);
    if (it != userlist.end())
    {
        return true;
    }
    return false;
}

bool    Channel::is_invited(const Client &client)
{
    if (invitelist.size() == 0)
        return false;
    std::vector<Client>::iterator it = std::find(invitelist.begin(), invitelist.end(), client);
    if (it != invitelist.end())
        return true;
    return false; 
}
