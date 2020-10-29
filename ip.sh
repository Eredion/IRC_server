cp inc/ft_irc.hpp inc/ft_irc.hpp.bck
export OURIP=$(ifconfig | grep "inet 16" | cut -f 2 -d ' ')
#echo $OURIP
sed "s/xxxIPxxx/$OURIP/g" inc/ft_irc.hpp.bck > inc/ft_irc.hpp
