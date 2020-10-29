#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Client
{
	friend class Server;
	friend class Channel;
	private:
		std::string nick;
		std::string username;
		std::string hostname;
		std::string realname;
		std::string servername;
		int hopcount;
		std::string token;
		std::string information;
		int socket;
		struct sockaddr_in info;
		std::string ip;
		std::string mode;
		std::string type;
		char	buff[512];
		int		count;
		std::string prefix;

		SSL *ssl;

		Client();
		Client &operator=(const Client &rhs);
	public:
		Client(int socket);
		Client(Client const &rhs);
		~Client();

		void	resetBuffer(void);
		int		getSocket() const;

		void	setIP(std::string const &ip);
		void	setSSL(SSL *ssl);
		int		setClient();
		void	make_prefix();
		
};

# endif
