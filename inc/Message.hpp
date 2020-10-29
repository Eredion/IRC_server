#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include "ft_irc.hpp"

class Client;

class Message
{
	friend class Server;

	private:
		std::string msg;
		std::string prefix;
		std::string command;
		int nparams;
		std::list<std::string> params;
		int socket;
		Client *orig;
	
		Message();
		Message(const Message &);
		Message &operator=(const Message &);

	public:
		Message(std::string const &msg, Client *orig);
		~Message();

		void printMsg(void);
};

#endif