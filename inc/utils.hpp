#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>


/* replies Message */

#define RPL_WELCOME(myclient)							"001 " + myclient + " :Welcome " + myclient + " to the ft_irc network"
#define RPL_CHANGENICK(myclient)						"001 " + myclient + " :You are now known as " + myclient
#define RPL_QUIT(myclient, message)						":" + myclient + " QUIT :Quit: " + message
#define RPL_PING(myclient, token)						":" + myclient + " PING :" + token
#define RPL_PONG(myclient, token)						":" + myclient + " PONG :" + token
#define RPL_CLOSECLIENT									": ERROR : you should be conected "
#define RPL_UMODEIS(myclient, modes)					"221 " + myclient + " :" + modes		
#define RPL_LISTSTART(myclient)							"321 " + myclient + " Channel :Users Name"
#define RPL_LIST(myclient, mychannel, client_count, topic) "322 " + myclient + " " + mychannel + " " + client_count + " :" + topic
#define RPL_LISTEND(myclient)							"323 " + myclient + " :End of /LIST"
#define RPL_CHANNELMODEIS(myclient, mychannel, mode_string, mode_args)	"324 " + myclient + " " + mychannel + " " + mode_string + mode_args
#define RPL_NOTOPIC(myclient, mychannel)				"331 " + myclient + " " + mychannel + " :No topic is set"
#define RPL_TOPIC(myclient, mychannel, channel_topic)	"332 " + myclient + " " + mychannel + " :" + channel_topic
#define RPL_NAMEREPLY(myclient, channel_symbol, channel, users)  	 	"353 " + myclient + " " + channel_symbol + " "  + channel + " :" + users
#define RPL_ENDOFNAMES(myclient, mychannel)				"366 " + myclient + " " + mychannel + " :End of /NAMES list"
#define RPL_PRIVMSG(myclient, target, message)			":" + myclient + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(myclient, target, message)			":" + myclient + " NOTICE " + target + " :" + message
#define RPL_MODE(myclient, channel, modes, args)		":" + myclient + " MODE " + channel + " " + modes + " " + args
#define RPL_INVITING(myclient, nickToInvite, mychannel)	":" + myclient + nickToInvite + mychannel
#define ERR_ALREADYINVITED(myclient, nickToInvite, mychannel)	": USER " + nickToInvite + "already invited to channel " + mychannel 
#define	RPL_UNBANNED(myclient, nickToBan, mychannel)	":" + myclient + " " + mychannel + " :"+ nickToBan + " has been unbanned"
#define	RPL_BANNED(myclient, nickToBan, mychannel)		":" + myclient + " " + mychannel + " :"+ nickToBan + " has been banned"
#define	CHANGEDNICKNAME(myclient, newNickname)			":" +  myclient + " NICK " + newNickname

/* Error Messages */

#define ERR_INVALIDCHANNELNAME(myclient, mychannel)			"476 " + myclient + " :Invalid channel name: " + mychannel
#define ERR_NOSUCHCHANNEL(myclient, mychannel)				"403 " + myclient + " " + mychannel + " :No such channel"
#define ERR_USERNOTINCHANNEL(myclient, target, mychannel) 	"441 " + myclient + " " + target + " " + mychannel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(myclient, mychannel)				"442 " + myclient + " " + mychannel + " :You're not on that channel"
#define ERR_NONICKNAMEGIVEN(myclient)						"431 " + myclient + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(nickname)					"433 " + nickname  + " :Nickname is already in use"
#define ERR_ERRONEUSNICKNAME(myclient, nickname)			"432 " + myclient + " " + nickname  + " :Erroneus nickname"
#define ERR_CMDNEEDMOREPARAMS(myclient, command)			"461 " + myclient + " " + command + "  :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(myclient, command)				"421 " + myclient + " " + command + " :Unknown command"
#define ERR_ALREADYAUTHENTIFIED(myclient)					"462 " + myclient + " :Already authentified"
#define ERR_WRONGPASSWORD(myclient)							"464 " + myclient + " :Password incorrect"
#define ERR_ALREADYREGISTERED(myclient)						"462 " + myclient + " :You may not reregister"
#define ERR_NOTAUTHENTIFIED(myclient)						"451 " + myclient + " :You are not authentified"
#define ERR_CHANOPRIVSNEEDED(myclient, mychannel)			"482 " + myclient + " " + mychannel + " :You're not channel operator"
#define ERR_USERONCHANNEL(myclient, nickToInvite, mychannel) "443 " + myclient + " " + mychannel + " :" + nickToInvite + " is already on channel"
#define	ERR_ALREADYBANNED(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already banned"
#define	ERR_ALREADYUNBANNED(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already unbanned"
#define	ERR_ALREADYEXCEPTEDFROMBAN(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already excepted from ban"
#define	ERR_ALREADYUNEXCEPTEDFROMBAN(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already unexcepted from ban"
#define	ERR_ALREADYEXCEPTEDFROMINVITE(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already excepted from invite-only"
#define	ERR_ALREADYUNEXCEPTEDFROMINVITE(myclient, nickToBan, mychannel)	"475 " + myclient + " " + mychannel + " :"+ nickToBan + " Already unexcepted from invite-only"
#define ERR_NOSUCHNICK(myclient, nickname)					"401 " + myclient + " " + nickname + " :No such nickname"
#define ERR_CANNOTSENDTOCHAN(myclient, channel)				"404 " + myclient + " " + channel + " :Cannot send to channel"
#define	ERR_CHANNELISFULL(myclient, channel)				"471 " + myclient + " " + channel + " :Cannot join channel (+l)"
#define	ERR_INVITEONLYCHAN(myclient, channel)				"473 " + myclient + " " + channel + " :Cannot join channel (+i)"
#define	ERR_BANNEDFROMCHAN(myclient, channel)				"474 " + myclient + " " + channel + " :Cannot join channel (+b)"
#define	ERR_BADCHANNELKEY(myclient, channel)				"475 " + myclient + " " + channel + " :Cannot join channel (+k)"
#define ERR_UMODEUNKNOWNFLAG(myclient)						"501 " + myclient + " :Unknown MODE flag"
#define ERR_USERSDONTMATCH(myclient)						"502 " + myclient + " :Cannot change mode for other users"

#define PING_INTERVAL 20 // seconds
#define TIMEOUT 20 // seconds
void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result);
void	sig_quit(int code);