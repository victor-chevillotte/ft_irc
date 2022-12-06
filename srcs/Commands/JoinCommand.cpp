#include "../../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client &client, std::string arguments)
{

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 1 || splited_args[0].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return;
	}

	if (arguments == "0")
	{
		_server->allChannelLeave(client, "");
		return;
	}
	// check if Channel exist
	std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(splited_args[0]);
	if (result.first)
	{
		// checks if client is already on channel
		if (result.second->isUser(client.getNickname()))
		{
			client.reply(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isMode('i') && !result.second->isInvited(client.getNickname()) && !result.second->isExceptedFromInvite(client.getNickname()))
		{
			client.reply(ERR_INVITEONLYCHAN(client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isMode('k') && (!(splited_args.size() > 1) || !result.second->isPassword(splited_args[1])))
		{
			client.reply(ERR_BADCHANNELKEY(client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isBanned(client.getNickname()) && !result.second->isExceptedFromBan(client.getNickname()))
		{
			client.reply(ERR_BANNEDFROMCHAN(client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isMode('l') && result.second->hasReachedClientsLimit())
		{
			client.reply(ERR_CHANNELISFULL(client.getNickname(), result.second->getName()));
			return;
		}

		// client actually join the channel and warn other users
		client.getJoinedChannelNb()++;
		result.second->addUser(client);
		sendJoinNotif(client, *result.second);
		return;
	}
	// check if channel_name is not valid
	if (invalidChannelName(splited_args[0]))
	{
		client.reply(ERR_INVALIDCHANNELNAME(client.getNickname(), splited_args[0]));
		return;
	}
	Channel new_channel(splited_args[0]);
	client.getJoinedChannelNb()++;
	Client client_copy = client;
	client_copy.setPtr(&client);
	client_copy.resetUserModes();
	client_copy.addUserMode('@');
	new_channel.addUser(client_copy);
	_server->addChannel(new_channel);
	sendJoinNotif(client, new_channel);
}

void JoinCommand::sendJoinNotif(Client &client, Channel channel)
{
	std::string prefix;
	prefix = client.getPrefix();
	if (!channel.isMode('q'))
		channel.broadcastMessage(":" + prefix + " JOIN " + channel.getName());
	if (!channel.getTopic().empty())
		client.reply(RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
	else
		client.reply(RPL_NOTOPIC(client.getNickname(), channel.getName()));
	client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getSymbol(), channel.getName(), channel.getUserList(true)));
	client.reply(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));
}

bool JoinCommand::invalidChannelName(const std::string &channel_name)
{
	if (channel_name.empty())
		return true;
	if (channel_name.size() > 50 || channel_name.size() < 2)
		return true;

	/*
	** restricted channel name creation to #channel only
	** normal condition would be :
	** if (channel_name[0] != '#' && channel_name[0] !=  '+' && channel_name[0] !=  '!' && channel_name[0] != '&')
	*/

	if (channel_name[0] != '#')
		return true;
	if (channel_name.find(" ") < channel_name.size() || channel_name.find(",") < channel_name.size() || channel_name.find("^G") < channel_name.size())
		return true;
	return false;
}