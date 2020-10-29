# include "../inc/Channel.hpp"

Channel::~Channel() {}

Channel::Channel(const std::string &name, const std::string &nick)
	: name(name), topic(""), mode("-a-i-m+n-q-p+s-r-t-k-l-b-e-I"), key("")
{
	if (name[0] == '&')
		local = true;
	else
		local = false;
	addClient(nick);
	setPermissions(nick, "+o");
}

Channel::Channel(const std::string &name)
	: name(name), topic(""), mode("-a-i-m+n-q-p+s-r-t-k-l-b-e-I"), key("")
{
	if (name[0] == '&')
		local = true;
	else
		local = false;
}

void	Channel::addClient(const std::string &nick)
{
	if (!isClient(nick) && !isBanned(nick))
		nicks.insert(std::pair<std::string, std::string>(nick, "-o-b"));
	else
	{
		std::cout << "Could not add client\n";
	}

}

void	Channel::addBanned(const std::string &nick)
{
	if (!isBanned(nick))
	{
		if (isClient(nick))
			eraseClient(nick);
		banned.push_back(nick);
	}
}

void	Channel::eraseClient(const std::string &nick)
{
	if (!isClient(nick))
		return;
	for(std::map<std::string, std::string>::iterator it = nicks.begin();
		it != nicks.end(); ++it)
	{
		if (it->first == nick)
		{
			nicks.erase(it);
			return ;
		}
	}
}

void	Channel::eraseBanned(const std::string &nick)
{
	if (!isBanned(nick))
		return;
	for(std::vector<std::string>::iterator it = banned.begin();
		it != banned.end(); ++it)
	{
		if (*it == nick)
		{
			it = banned.erase(it);
			return ;
		}
	}
}

bool	Channel::isClient(const std::string &nick)
{
	for(std::map<std::string, std::string>::iterator it = nicks.begin();
		it != nicks.end(); ++it)
	{
		if (it->first == nick)
			return (1);
	}
	return (0);
}

bool	Channel::isBanned(const std::string &nick)
{
	for(std::vector<std::string>::iterator it = banned.begin();
		it != banned.end(); ++it)
	{
		if (*it == nick)
			return (1);
	}
	return (0);
}

const std::string Channel::get_permissions(const std::string &nick)
{
	if (!isClient(nick))
		return (NULL);
	for (std::map<std::string, std::string>::iterator it = nicks.begin();
		it != nicks.end(); ++it)
	{
		if (it->first == nick)
			return (it->second);
	}
	return (NULL);
}


void	Channel::setPermissions(const std::string &nick, const std::string &perm)
{
	//perm solo admite un cambio de modo (+x a -x, no -a+e+x+e)
	std::map<std::string, std::string>::iterator it = nicks.find(nick);
	if (it == nicks.end())
		return;
	int pos = it->second.find(perm[1]);
	it->second.replace(pos - 1, 1, perm.substr(0, 1));
}

void	Channel::setMode(const std::string &mode)
{
	int pos = mode.find(mode[1]);
	this->mode.replace(pos - 1, 1, mode.substr(0, 1));
}

void Channel::printChannel()
{
	for (std::map<std::string, std::string>::iterator it = nicks.begin();
		it != nicks.end(); ++it)
	{
		std::cout << "-:" << it->first << "\t" << it->second << "\t\n";
	}
}

void Channel::setKey(const std::string &key)
{
	this->key = key;
	setMode("+k");
}

std::string Channel::sendUserList(std::string serverip, std::string nick)
{
	std::string a[] = {
		":", serverip, " ", RPL_NAMREPLY, " ", nick, " @ ",
		this->name, " :", "NULL"};
	std::string t = buildString(a);
	for (std::map<std::string, std::string>::iterator it = nicks.begin();
		it != nicks.end(); ++it)
	{
		std::cout << "NICK" << (*it).first << std::endl;
		if ((*it).second.at(0) == '+')
			t.append("@");
		t.append((*it).first);
		t.append(" ");
	}
	return (t);
}

void	Channel::change_nick(const std::string &old_nick, const std::string &nick)
{
	std::cout<< old_nick << nick << std::endl;
	if (isClient(old_nick))
	{
		nicks.insert(std::pair<std::string, std::string>(nick, get_permissions(old_nick)));
		eraseClient(old_nick);
	}
	if (isBanned(old_nick))
	{
		for (std::vector<std::string>::iterator it = banned.begin();
			it != banned.end(); ++it)
		{
			if (*it == old_nick)
			{
				it = banned.erase(it);
				banned.push_back(nick);
			}
		}
	}
}
