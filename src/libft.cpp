#include "../inc/ft_irc.hpp"

void ft_perror(const char *s)
{
	std::cerr << s << std::endl;
	exit(EXIT_FAILURE);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	str = (char*)s;
	i = 0;
	while (i < n)
	{
		str[i] = c;
		i++;
	}
	return (str);
}

size_t ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s && s[i])
		++i;
	return (i);
}

int	ft_atoi(const char *str)
{
	unsigned long	res;
	unsigned long	limits;
	int				negative;

	negative = 1;
	res = 0;
	limits = (unsigned long)(9223372036854775807 / 10);
	while (*str && *str == ' ')
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		if ((res > limits || (res == limits && (*str - '0') > 7))
				&& negative == 1)
			return (-1);
		else if ((res > limits || (res == limits && (*str - '0') > 8))
				&& negative == -1)
			return (0);
		res = res * 10 + (*str - 48);
		++str;
	}
	return ((int)res * negative);
}

std::string ft_itoa(int nbr)
{
	long	n;
	size_t	len;
	char	*str;
	std::string s;

	n = nbr;
	len = (n > 0) ? 0 : 1;
	n = (n > 0) ? n : -n;
	while (nbr)
		nbr = len++ ? nbr / 10 : nbr / 10;
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	*(str + len--) = '\0';
	while (n > 0)
	{
		*(str + len--) = n % 10 + '0';
		n /= 10;
	}
	if (len == 0 && str[1] == '\0')
		*(str + len) = '0';
	if (len == 0 && str[1] != '\0')
		*(str + len) = '-';
	s = str;
	free(str);
	return (s);
}

int	ft_isalnum(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}