#include "../../inc/Command.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server) {}

PrivMsgCommand::~PrivMsgCommand(){};

void PrivMsgCommand::execute(Client &client, std::string arguments)
{

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
		return;
	}

	std::string target = splited_args[0];
	std::string message;

	for (std::vector<std::string>::iterator it = splited_args.begin() + 1; it != splited_args.end(); it++)
	{
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;
	if (target.at(0) == '#')
	{
		Channel* channel;
		channel = _server->getChannel(target);
		if (!channel)
		{
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target));
			return;
		}

		// checks if 'no external msg mode' is enabled
		if (channel->isMode('n'))
		{
			if (!(channel->isUser(client.getNickname())))
			{
				client.reply(ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
				return;
			}
		}

		// checks if 'moderated mode' is enabled
		if (channel->isMode('m') && (!channel->isOp(client.getNickname()) && !channel->isClientMode(client.getNickname(), '+')))
		{
			client.reply(ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
			return;
		}
		std::string	prefix = channel->isMode('a') ? "anonymous!anonymous@anonymous." : client.getPrefix();
		channel->broadcastExceptSource(RPL_PRIVMSG(prefix, target, message), client.getNickname());
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest)
	{
		client.reply(ERR_NOSUCHNICK(client.getNickname(), target));
		return;
	}

	dest->write(RPL_PRIVMSG(client.getPrefix(), target, message));
}