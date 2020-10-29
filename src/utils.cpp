#include "../inc/ft_irc.hpp"

void print(const std::string &str)
{
	std::cout << str << std::endl;
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t initialPos = 0;

	while (txt.at(initialPos) == ch)
		initialPos++;
	size_t pos = txt.find( ch , initialPos);
    strs.clear();
    while( pos != std::string::npos )
	{
        strs.push_back(txt.substr(initialPos, pos - initialPos));
		while (txt.at(initialPos) != ch)
			++initialPos;
        while (txt.at(initialPos) == ch)
		{
			initialPos++;
			if (initialPos == txt.length())
				break;
		}
        pos = txt.find( ch, initialPos );
    }
	if (txt.at(txt.length() - 1) != ch)
    	strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );
    return strs.size();
}

size_t split(const std::string &txt, std::list<std::string> &strs, char ch)
{
    size_t initialPos = 0;

	while (txt.at(initialPos) == ch)
		initialPos++;
	size_t pos = txt.find( ch , initialPos);
    strs.clear();
    while( pos != std::string::npos )
	{
        strs.push_back(txt.substr(initialPos, pos - initialPos));
		while (txt.at(initialPos) != ch)
			++initialPos;
        while (txt.at(initialPos) == ch)
		{
			initialPos++;
			if (initialPos == txt.length())
				break;
		}
        pos = txt.find( ch, initialPos );
    }
	if (txt.at(txt.length() - 1) != ch)
    	strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );
    return strs.size();
}


void print_vector(std::vector<std::string> vct)
{
	for (std::vector<std::string>::iterator it = vct.begin(); it != vct.end(); ++it)
		std::cout << "-" << *it << std::endl;
}

void print_list(std::list<std::string> vct)
{
	for (std::list<std::string>::iterator it = vct.begin(); it != vct.end(); ++it)
		std::cout << "-" << *it << std::endl;
}

std::string buildString(std::string strs[])
{
	//el último elemento de strs debe de ser una string "NULL" con comillas;
	std::string res;

	for (int i = 0; strs[i] != "NULL"; ++i)
		res.append(strs[i]);
	return res;
}

void ft_wait(double sec)
{
	double min = -sec * 2e8;
	double max = -min;
	while (min != max)
		++min;
}

std::string parse_user(std::string const &str)
{
	int begin = 0;
	int end = 0;
	if (str.length() == 0)
		return "";
	while (ft_isalnum(str.at(begin)) == 0)
		begin++;
	end = begin;
	while(end < (int)str.length() && ft_isalnum(str.at(end)))
		end++;
	if (end > (int)str.length() - 1)
		end = str.length();
	return str.substr(begin, end - begin);
}