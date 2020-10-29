#ifndef FT_IRC_HPP
# define FT_IRC_HPP
#ifndef OPSWD
# define OPSWD "ft_irc2020"
#endif
# define WAITTIME 0.05
# define SSLPORT "6669"
# define IPADDRESS "xxxIPxxx" 
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <iostream>
# include <string>
# include <string.h>
# include <fcntl.h>
# include <sys/time.h>
# include <vector>
# include <list>
# include <map>
# include <algorithm>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include "reply.hpp"
# include "Server.hpp"
# include "Message.hpp"
# include "Client.hpp"
# include "Channel.hpp"

void		ft_perror(const char *s);
void		*ft_memset(void *s, int c, size_t n);
size_t		ft_strlen(const char *s);
int			ft_atoi(const char *str);
std::string	ft_itoa(int nbr);
int			ft_isalnum(int c);
void		ft_wait(double sec);
void		print(std::string const &str);
size_t		split(const std::string &txt, std::vector<std::string> &strs, char ch);
size_t		split(const std::string &txt, std::list<std::string> &strs, char ch);
void		print_vector(std::vector<std::string> vct);
void		print_list(std::list<std::string> lst);
std::string	buildString(std::string strs[]);
std::string parse_user(std::string const &str);
#endif
