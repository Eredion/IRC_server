#include "../inc/Client.hpp"

Client::Client(int socket)
	:socket(socket), count(0)
{
	ft_memset(buff, 0, sizeof(buff));
	mode = "-a+i-w-r-o-O-s";
	type = "unknown";
	servername = "ft_irc@42madrid.com";
	std::cout << "Unknown client " << socket << " created.\n";
}

Client::~Client()
{
	close(socket);
}

Client::Client(const Client &rhs)
	:socket(rhs.socket), info(rhs.info), mode(rhs.mode), type(rhs.type), \
			count(rhs.count)
{
	ft_memset(buff, 0, sizeof(buff));
}

void Client::resetBuffer(void)
{
	ft_memset(this->buff, 0, 512);
	this->count = 0;
}

int Client::getSocket() const
{
	return this->socket;
}

void Client::setIP(const std::string &ip)
{
	this->ip = ip;
}

int Client::setClient()
{
	if (nick.length() > 0 && username.length() > 0)
		type = "client";
	else
		return -1;
	make_prefix();
	prefix = std::string(":").append(nick);
	prefix.append("!");
	prefix.append(username);
	prefix.append("@");
	prefix.append(ip);
	return (0);
}

void Client::make_prefix()
{
	prefix.clear();
	prefix = std::string(":").append(nick);
	prefix.append("!~");
	prefix.append(username);
	return;
}
