#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Server;
class Message;
class Client;

class Channel
{
	friend class Server;
	private:
		std::string name;
		std::string topic;
		std::map<std::string, std::string> nicks;
		std::vector<std::string> banned;
		std::string mode;
		std::string key;
		bool local;

		Channel();
		Channel	&operator=(const Channel &rhs);
	public:
		Channel(const std::string &name, std::string const &nick);
		Channel(const std::string &name);
		~Channel();

		void	addClient(const std::string &nick);
		void	addBanned(const std::string &nick);
		void	eraseClient(const std::string &nick);
		void	eraseBanned(const std::string &nick);

		bool	isClient(const std::string &nick);
		bool	isBanned(const std::string &nick);

		const std::string get_permissions(const std::string &nick);

		void	setPermissions(const std::string &nick, const std::string &perm);

		void	printChannel();
		void	setMode(std::string const &mode);
		void	setKey(std::string const &key);
		std::string	sendUserList(std::string serverip, std::string nick);

		void	change_nick(const std::string &old_nick, const std::string &nick);

};


#endif
