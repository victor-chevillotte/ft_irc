#include "../../inc/Command.hpp"

InviteCommand::InviteCommand(Server *server) : Command(server) {}

InviteCommand::~InviteCommand(){};

void InviteCommand::execute(Client &client, std::string arguments)
{

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "INVITE"));
		return;
	}

	Channel *channel;
	channel = _server->getChannel(splited_args[1]);

	if (!channel)
	{
		client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), splited_args[1]));
		return;
	}
	if (!channel->isUser(client.getNickname()))
	{
		client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
		return;
	}

	if (!channel->isOp(client.getNickname()))
	{
		client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
		return;
	}

	if (channel->isUser(splited_args[0]))
	{
		client.reply(ERR_USERONCHANNEL(client.getNickname(), splited_args[0], channel->getName()));
		return;
	}

	if (channel->isInvited(splited_args[0]))
	{
		client.reply(ERR_ALREADYINVITED(client.getNickname(), splited_args[0], channel->getName()));
		return;
	}
	channel->addInvitation(splited_args[0]);
	channel->broadcastMessage(RPL_INVITING(client.getNickname(), splited_args[0], channel->getName()));
}