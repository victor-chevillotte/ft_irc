#include "../../inc/Command.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {};

NoticeCommand::~NoticeCommand() {};

void NoticeCommand::execute(Client &client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		return;
	}

	std::string target = splited_args[0];
	std::string message;

	for (std::vector<std::string>::iterator it = splited_args.begin() + 1; it != splited_args.end(); it++)
	{
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;
	if (target.at(0) == '#') {

		Channel* channel;
		channel = _server->getChannel(target);
		if (!channel)
		{
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), splited_args[1]));
			return;
		}
		if (channel->isMode('n'))
		{
			if (!(channel->isUser(client.getNickname())))
			{
				client.reply(ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
				return;
			}
		}
		channel->broadcastExceptSource(RPL_NOTICE(client.getPrefix(), target, message), client.getPrefix());
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest)
	{
		return;
	}
	dest->write(RPL_NOTICE(client.getPrefix(), target, message));
}