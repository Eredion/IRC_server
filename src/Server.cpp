#include "../inc/Server.hpp"

Server::Server(int argc, char **argv)
	:  msgofday("HELLO_MADRID"), creationtime("02/09/2019 09:00:00 GMT+1")
{
	int i;
	int j;
	std::string argv1;

	if (argc < 3 || argc > 4)
	{
		std::cout << ("Error:\nArguments. USAGE: ");
		std::cout << "./ircserv [host:port_network:password_network] <port> <password>\n";
		exit(EXIT_FAILURE);
	}
	this->port = std::string(argv[argc - 2]);
	this->password = std::string(argv[argc - 1]);

	if (argc == 4)
	{
		argv1 = std::string(argv[1]);
		i = argv1.find(':');
		this->host = argv1.substr(0, i);
		j = argv1.find(":", i + 1);
		this->port_network = argv1.substr(i + 1, j - i - 1);
		this->password_network = argv1.substr(j + 1, argv1.length() - 1);
	}
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
}

Server::~Server()
{
}

void Server::clear()
{
	for (std::vector<Client*>::iterator it= clients.begin();
		it!= clients.end(); )
	{
		if ((*it)->type == "client")
		{
			Message msg(":loop QUIT", *it);
			++it;
			quit(msg);
			
		}
		else
		{
			++it;
		}
		
	}
}

std::string Server::getIp(void)
{
	return IPADDRESS;
}

void Server::init_server()
{
	struct addrinfo	hints;
	struct addrinfo	*ai = NULL;
	struct addrinfo *p;
	int				yes = 1;
	int 			rv;

	fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	ft_memset(&hints, 0, sizeof hints);
	//hints.ai_family = PF_INET; //IPV4
	hints.ai_family = AF_UNSPEC; //IPV4 + IPV6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(0, this->port.c_str(), &hints, &ai)) != 0)
		ft_perror(strerror(rv));
	this->ip = getIp();
	for (p = ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
			continue;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue;
		}
		break;
	}
	if (p == NULL)
		ft_perror("Error: failed to bind.");
	if (listen(listener, 10) == -1)
		ft_perror("Error: Could not listen.");
	print("Server connected.");
	print(getInfo());
	FD_SET(listener, &master);
	if (port == SSLPORT)
		ssl_init();
	this->fdmax = this->listener;
	freeaddrinfo(ai);
}

void Server::serv_connect()
{
	int sockfd;
	std::string buff;
	struct addrinfo hints;
	struct addrinfo *ai = NULL;
	struct addrinfo *p;
	int rv;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(this->host.c_str(), this->port_network.c_str(), &hints, &ai)) != 0)
		ft_perror(strerror(rv));
	for (p = ai; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd < 0)
			continue;
		if (::connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            ft_perror("Server to server: could not connect");
            continue;
        }
		break;
	}
	if (p == NULL)
        ft_perror("failed to connect\n");
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	FD_SET(sockfd, &master);
	if (sockfd > fdmax)
		fdmax = sockfd;
	Client *tmp = new Client(sockfd);
	tmp->setIP(host);
	tmp->hopcount = 1;
	tmp->type = "server";
	clients.push_back(tmp);
}


void Server::main_loop()
{
	if (port != SSLPORT)
	{
		while(42)
		{
			serv_select();
			for (int i = 0; i <= fdmax; ++i)
			{
				if (FD_ISSET(i, &read_fds))
				{
					if (i == listener)
						new_connection();
					else
						receive(i);
				}
			}
		}
	}
	else
	{
		while(42)
		{
			serv_select();
			for (int i = 0; i <= fdmax; ++i)
			{
				if (FD_ISSET(i, &read_fds))
				{
					if (i == listener)
						ssl_new_connection();
					else
						ssl_receive(i);
				}
			}
		}
	}
	
}

void Server::new_connection()
{
	int					newfd;
	struct sockaddr_in	remoteaddr;
	socklen_t			addrlen = sizeof(struct sockaddr_in);

	ft_memset(&remoteaddr, 0, sizeof(remoteaddr));
	ft_memset(&(remoteaddr.sin_addr), 0, sizeof(remoteaddr.sin_addr));
	newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
	if (newfd == -1)
	{
		std::cerr << ("Error: Accept.") << std::endl;
		return ;
	}
	fcntl(newfd, F_SETFL, O_NONBLOCK);
	FD_SET(newfd, &master);
	if (newfd > fdmax)
		fdmax = newfd;
	Client *tmp = new Client(newfd);
	tmp->setIP(IPADDRESS);
	clients.push_back(tmp);
	sendmsg(tmp->socket, "NOTICE * : PLEASE LOG IN");
	std::cout << "New connection on socket " << newfd << std::endl;
}

void	Server::serv_select()
{
	struct timeval timeout;

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	read_fds = master;
	if (select(fdmax + 1, &read_fds, STDIN_FILENO, NULL, &timeout) == -1)
		ft_perror("Error: could not call select.");
}

std::string
Server::getInfo(void) const
{
	std::string ret;

	ret.append("Host: ");
	ret.append(this->host);
	ret.append(" Version: 0.1beta - Protocol 2.10 - 42 Madrid");
	ret.append(" ");
	ret.append(this->creationtime);
	return (ret);
}

void Server::sendmsg(int socket, std::string const &str)
{
	std::string tmp;

	tmp = str.substr(0, 510);
	tmp.append("\r");
	if (str.find("\n") == std::string::npos)
		tmp.append("\n");
	if (FD_ISSET(socket, &master))
	{
		if (port == SSLPORT)
		{
			SSL *ssl = getClient(socket)->ssl;
			int ret = SSL_write(ssl, tmp.c_str(), ft_strlen(tmp.c_str()));
			if (ret <= 0)
				print("ssl write error");
		}
		else if ((send(socket, tmp.c_str(), ft_strlen(tmp.c_str()), 0)) == -1)
			std::cerr << "Error sending info to " << socket << std::endl;
	}
	else
		std::cerr << "Sendmsg: error: FD " << socket << " is not set\n";
}

void Server::receive(int socket)
{
	int rec = 0;
	Client *tmp;

	tmp = getClient(socket);
	if ((rec = recv(socket, &tmp->buff[tmp->count], 512 - tmp->count, 0)) >= 0)
	{
		tmp->count += rec;
	}
	if (rec == -1)
	{
		print(strerror(rec));
		std::cerr << "Error: recv returned -1" << std::endl;
	}
	else if (rec == 0)
	{
		std::cerr << "Error: socket " << socket << " disconnected\n";
		close(socket);
		deleteClient(socket);
		FD_CLR(socket, &master);
	}
	if (std::string(tmp->buff).find("\n") != std::string::npos || tmp->count >= 512)
	{
		std::cout << "Received " << tmp->buff << " from " << tmp->socket<< "\n";
		Message msg(tmp->buff, tmp);
		exec(msg);
		tmp->resetBuffer();
	}
}

void Server::receive_noexec(int socket)
{
	int rec = 0;
	Client *tmp;

	tmp = getClient(socket);
	if ((rec = recv(socket, &tmp->buff[tmp->count], 512 - tmp->count, 0)) >= 0)
	{
		tmp->count += rec;
	}
	if (rec == -1)
	{
		print(strerror(rec));
		std::cerr << "Error: recv returned -1" << std::endl;
	}
	else if (rec == 0)
	{
		std::cerr << "Error: socket " << socket << " disconnected\n";
		close(socket);
		deleteClient(socket);
		FD_CLR(socket, &master);
	}
	if (std::string(tmp->buff).find("\n") != std::string::npos || tmp->count >= 512)
	{
		std::cout << "Received " << tmp->buff << " from " << tmp->socket<< "\n";
		tmp->resetBuffer();
	}
}

Client *Server::getClient(int socket)
{
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator itt = clients.end();

	while (it != itt)
	{
		if ((*it)->socket == socket)
			return *it;
		++it;
	}
	return NULL;
}

Channel &Server::getChannel(std::string const &ch)
{
	for (std::vector<Channel>::iterator it = channels.begin();
		it != channels.end(); it++)
	{
		if ((*it).name == ch)
			return *it;
	}
	return *channels.end();
}

Channel &Server::getOtherChannel(std::string const &ch)
{
	if (otherchannels.size() < 1)
		return *otherchannels.end();
	for (std::vector<Channel>::iterator it = otherchannels.begin();
		it != otherchannels.end(); it++)
	{
		if (it->name == ch)
			return *it;
	}
	return *otherchannels.end();
}

void Server::deleteClient(int socket)
{
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator itt = clients.end();

	while (it != itt)
	{
		if ((*it)->socket == socket)
		{
			delete *it;
			clients.erase(it);
		}
		++it;
	}
}

std::vector<Channel>::iterator Server::exists(std::string &channel)
{
	std::vector<Channel>::iterator it = this->channels.begin();
	while (it != this->channels.end())
	{
		if (it->name == channel)
			return it;
		++it;
	}
	return this->channels.end();
}

void Server::forward(std::string const &str)
{
	for (std::vector<Client*>::iterator it = clients.begin();
		it != clients.end(); ++it)
	{
		if ((*it)->type == "server" || (*it)->type == "preserver")
		{
			sendmsg((*it)->socket, str);
		}
	}
}

void Server::broadcast(
		std::string const &orig, std::string const &rpl, std::string const &msg)
{
	std::string arr[] = {":", orig, " ", rpl, " ", msg, "NULL"};
	std::string str = buildString(arr);
	for (std::vector<Client*>::iterator it = clients.begin();
		it != clients.end(); ++it)
	{
		if ((*it)->type == "server")
			sendmsg((*it)->socket, str);
	}
}



void Server::tochannel(std::string const &channel, std::string const &msg, int orig)
{
	Channel c = getChannel(channel);
	if (c.name.length() > 0)
	{
		for (std::map<std::string, std::string>::iterator it = c.nicks.begin();
			it != c.nicks.end(); ++it)
		{
			
			int socket = getClient((*it).first)->socket;
			if (socket != orig)
			{
				sendmsg(socket, msg);
			}
		}
	}
/* 	if (getClient(orig)->type == "server")
	{
		print("forwarded privmsg to channel");
		return;
	}
	Channel oc = getOtherChannel(channel);
	if (oc.name.length() > 1)
	{
		print("forwarding privmsg to channel");
		std::string fwd[] = {msg, "NULL"};
		forward(buildString(fwd));
	} */
}

void Server::tochannel(Channel &channel, Message &msg)
{
	if (msg.orig->type == "client")
	{
		std::string fwd[] = {msg.orig->prefix, " ", msg.msg, "NULL"};
		for (std::map<std::string, std::string>::iterator it = channel.nicks.begin();
			it != channel.nicks.end(); ++it)
		{
			int socket = getClient((*it).first)->socket;
			if (socket != msg.socket)
			{
				sendmsg(socket, buildString(fwd));
			}
		}
		forward(buildString(fwd));
	}
}

void Server::not_params(Message &msg)
{
	std::string words[] = {this->ip, " ", ERR_NEEDMOREPARAMS, " ", \
	msg.orig->nick, " ", msg.command, " :Not enough parameters", "NULL" };
	sendmsg(msg.orig->socket, buildString(words));
}

void Server::no_such_channel(Message &msg)
{
	std::string words[] = {":", this->ip, " ", ERR_NOSUCHCHANNEL, " ", \
	msg.orig->nick, " ", *msg.params.begin(), " :No such channel", "NULL" };
	sendmsg(msg.orig->socket, buildString(words));
}

void Server::no_such_server(Message &msg)
{
	std::string words[] = {":", this->ip, " ", ERR_NOSUCHSERVER, " ", \
	msg.orig->nick, " ", *msg.params.begin(), " :No such server", "NULL" };
	sendmsg(msg.orig->socket, buildString(words));
}

void Server::not_in_channel(Message &msg)
{
	std::string words[] = {":", this->ip, " ", ERR_NOTONCHANNEL, " ", \
	msg.orig->nick, " ", *msg.params.begin(), " :You're not on that channel",
	"NULL" };
	sendmsg(msg.orig->socket, buildString(words));
}

void Server::not_operator(Message &msg)
{
	std::string words[] = {":", this->ip, " ", ERR_CHANOPRIVSNEEDED, " ", \
	msg.orig->nick, " ", *msg.params.begin(), " :You're not an operator of this channel",
	"NULL" };
	sendmsg(msg.orig->socket, buildString(words));
}

void Server::no_such_nick_channel(Message &msg)
{
	std::string words[] = {":", ip, " ", ERR_NOSUCHNICK, " ", msg.orig->nick, \
	" ", *msg.params.begin(), " :No such nick/channel", "NULL"};
	sendmsg(msg.socket, buildString(words));
}

void Server::unknown_command(Message &msg)
{
	if (msg.orig->type == "server")
		return;
	std::string strs[] = {":", ip, " ", ERR_UNKNOWNCOMMAND, " ",\
	msg.orig->nick, " ", msg.command, " :Unknown command", "NULL"};
	std::string reply = buildString(strs);
	sendmsg(msg.orig->socket, reply);
}


void Server::welcome(const Client &cli)
{
	std::string newuser("NEWUSER ");
	newuser.append(cli.nick);
	forward(newuser);
	std::string words[] = {":", ip, " ", RPL_WELCOME, " ", cli.nick, \
	" :Welcome to our ft_IRC from asegovia and darodrig ", cli.nick, "NULL"};
	sendmsg(cli.socket, buildString(words));
	std::string words2[] = {":", ip, " ", RPL_YOURHOST, " ", cli.nick, \
	" :Your host is ", host, "[", ip, "/", port, "], running version ft_irc-0.5", "NULL"};
	sendmsg(cli.socket, buildString(words2));
	std::string words3[] = {":", ip, " ", RPL_CREATED, " ", cli.nick, \
	" :This server was created Mon Nov 4 2019 at 09:00:00 UTC + 1", "NULL"};
	sendmsg(cli.socket, buildString(words3));
	std::string words4[] = {":", ip, " ", RPL_MYINFO, " ", cli.nick, \
	" ft_irc-0.5 ", "NULL"}; //Aqui van los posibles modos de usuario. 
	sendmsg(cli.socket, buildString(words4));
	std::string words5[] = {":", ip, " ", "005", " ", cli.nick, \
	" CHARSET=ascii", "NULL"}; //Aqui van otras cosas soportadas, longitud maxima de nick...
	sendmsg(cli.socket, buildString(words5));
	Client *cll = getClient(cli.nick);
	lusers(cll);
	motd(cll);
}

void Server::sendusers(Message &msg)
{

	ft_wait(WAITTIME);
	int users = 0;
	for (std::vector<Client*>::iterator it = this->clients.begin();
		it != this->clients.end(); ++it)
	{
		if ((*it)->type == "client")
			++users;
	}
	if (users > 0)
	{
		std::string users = "NEWUSER ";
		for (std::vector<Client*>::iterator it = clients.begin();
			it != clients.end(); ++it)
		{
			if ((*it)->type == "client")
			{
				users.append((*it)->nick);
				users.append(" ");
			}
		}
		sendmsg(msg.socket, users);
	}

	ft_wait(WAITTIME);
	int oth = 0;
	for (std::vector<std::string>::iterator it = this->otherclients.begin();
		it != this->otherclients.end(); ++it)
	{
		++oth;
	}
	if (oth > 0)
	{
		std::string others = "NEWUSER ";
		for (std::vector<std::string>::iterator it = otherclients.begin();
			it != otherclients.end(); ++it)
		{
			others.append(*it);
			others.append(" ");
		}
		sendmsg(msg.socket, others);
	}
}

void Server::sendchannels(Message &msg)
{
	if (channels.size() != 0)
	{
		for (std::vector<Channel>::iterator it = channels.begin();
			it != channels.end(); ++it)
		{
			if ((it)->name.at(0) == '&')
				continue;
			std::string users = "ADDCHANNEL ";
			users.append(it->name);
			users.append(" ");
			for (std::map<std::string, std::string>::iterator itt = it->nicks.begin();
				itt != it->nicks.end(); ++itt)
				{
					users.append(itt->first);
					users.append(" ");
				}
			ft_wait(WAITTIME);
			sendmsg(msg.socket, users);
		}
	}
	if (otherchannels.size() != 0)
	{
		for (std::vector<Channel>::iterator it = otherchannels.begin();
			it != otherchannels.end(); ++it)
		{
			std::string users = "ADDCHANNEL ";
			users.append(it->name);
			users.append(" ");
			for (std::map<std::string, std::string>::iterator itt = it->nicks.begin();
				itt != it->nicks.end(); ++itt)
				{
					users.append(itt->first);
					users.append(" ");
				}
			ft_wait(WAITTIME);
			sendmsg(msg.socket, users);
		}
	}
}

void Server::partother(std::string const &nick)
{
	for (std::vector<Channel>::iterator it = otherchannels.begin();
		it!= otherchannels.end(); ++it)
	{
		it->eraseClient(nick);
	}
}

void Server::quitother(std::string const &nick)
{
	for (std::vector<std::string>::iterator it = otherclients.begin();
		it != otherclients.end(); ++it)
	{
		if (*it == nick)
		{
			otherclients.erase(it);
			break;
		}
	}
	partother(nick);
}

bool Server::isServer(std::string const &nick)
{
	if (nick == port)
		return 1;
	for (std::vector<Client*>::iterator it = clients.begin();\
		it != clients.end(); ++it)
	{
		if ((*it)->nick == nick && (*it)->type == "server")
			return 1;
	}
	return 0;
}
