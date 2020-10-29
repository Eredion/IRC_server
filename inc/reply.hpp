
#ifndef REPLY_H_
#define REPLY_H_

#define RPL_WELCOME             "001"
#define RPL_YOURHOST            "002"
#define RPL_CREATED             "003"
#define RPL_MYINFO              "004"

#define RPL_TRACEUSER			"205"
#define RPL_TRACESERVER			"206"
#define RPL_ENDOFTRACE			"262"

#define RPL_STATSCOMMANDS		"212"
#define RPL_ENDOFSTATS			"219"
#define RPL_STATSOLINE			"243"

#define RPL_LUSERCLIENT         "251"
#define RPL_LUSEROP             "252"
#define RPL_LUSERUNKNOWN        "253"
#define RPL_LUSERCHANNELS       "254"
#define RPL_LUSERME             "255"

#define RPL_ADMINME 			"256"
#define RPL_ADMINLOC1 			"257"
#define RPL_ADMINLOC2 			"258"
#define RPL_ADMINEMAIL			"259"

#define RPL_AWAY                "301"
#define RPL_UNAWAY              "305"
#define RPL_NOWAWAY             "306"

#define RPL_WHOISUSER           "311"
#define RPL_WHOISSERVER         "312"
#define RPL_WHOISOPERATOR       "313"
#define RPL_WHOISIDLE           "317"
#define RPL_ENDOFWHOIS          "318"
#define RPL_WHOISCHANNELS       "319"

#define RPL_WHOREPLY            "352"
#define RPL_ENDOFWHO            "315"

#define RPL_LISTSTART			"321"
#define RPL_LIST                "322"
#define RPL_LISTEND             "323"

#define RPL_CHANNELMODEIS       "324"

#define RPL_NOTOPIC             "331"
#define RPL_TOPIC               "332"

#define RPL_VERSION				"351"
#define RPL_NAMREPLY            "353"
#define RPL_LINKS				"364"
#define RPL_ENDOFLINKS			"365"
#define RPL_ENDOFNAMES          "366"

#define RPL_INFO				"371"
#define RPL_ENDOFINFO			"374"
#define RPL_MOTDSTART           "375"
#define RPL_MOTD                "372"
#define RPL_ENDOFMOTD           "376"

#define RPL_YOUREOPER           "381"

#define RPL_TIME				"391"

#define ERR_NOSUCHNICK          "401"
#define ERR_NOSUCHSERVER        "402"
#define ERR_NOSUCHCHANNEL       "403"
#define ERR_CANNOTSENDTOCHAN    "404"
#define ERR_NORECIPIENT         "411"
#define ERR_NOTEXTTOSEND        "412"
#define ERR_UNKNOWNCOMMAND      "421"
#define ERR_NOMOTD              "422"
#define ERR_NONICKNAMEGIVEN     "431"
#define ERR_NICKNAMEINUSE       "433"
#define ERR_USERNOTINCHANNEL    "441"
#define ERR_NOTONCHANNEL        "442"
#define ERR_NOTREGISTERED       "451"
#define ERR_NEEDMOREPARAMS      "461"
#define ERR_ALREADYREGISTRED    "462"
#define ERR_PASSWDMISMATCH      "464"
#define ERR_UNKNOWNMODE         "472"
#define ERR_NOPRIVILEGES        "481"
#define ERR_CHANOPRIVSNEEDED    "482"
#define ERR_NOOPERPRIVILEGES	"491"
#define ERR_UMODEUNKNOWNFLAG    "501"
#define ERR_USERSDONTMATCH      "502"

#endif
