#include "../inc/Message.hpp"

Message::Message(std::string const &msg, Client *orig)
	:msg(msg), socket(orig->getSocket()), orig(orig)
{
	if (msg.length() < 3)
	{
		command = "NULL";
		return;
	}
	split(msg.substr(0, msg.length() - 2), params, ' ');
	if (params.begin()->at(0) == ':')
	{
		prefix = *params.begin();
		params.pop_front();
	}
	
	command = *params.begin();
	if (params.size())
		params.pop_front();
	
}

Message::~Message(){}

void Message::printMsg(void)
{
	std::cout << "PR:" <<this->prefix << std::endl;
	std::cout << "CO:" <<this->command << std::endl;
	print_list(this->params);
}