#include "../../inc/Command.hpp"

TopicCommand::TopicCommand(Server *server) : Command(server) {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(Client& client, std::string arguments) {
	
	std::string channel_name;
	std::string new_topic;
	std::pair<bool, std::vector<Channel>::iterator> result;
	
	size_t pos = arguments.find(":");
	if (pos < arguments.size())
	{
		// multiple arguments case : set new topic
		new_topic = arguments.substr(pos + 1);
		channel_name = arguments.substr(0, pos - 1);
		result = _server->searchChannel(channel_name);
		bool found_channel = result.first;
		std::vector<Channel>::iterator chan_iter = result.second;

		// checks if user is on channel
		if (!found_channel)
		{
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
			return ;
		}

		if(!chan_iter->isUser(client.getNickname()))
		{
			client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel_name));
			return ;
		}

		// checks if 't mode is enabled' and if user has rights to change topic
		if (chan_iter->isMode('t') && !chan_iter->isClientMode(client.getNickname(), '@'))
		{
			client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel_name));
			return ;
		}

		// sets new topic
		chan_iter->setTopic(new_topic);
		chan_iter->broadcastMessage(":" + client.getPrefix() + " TOPIC " + arguments);
	}
	else
	{
		// single argument case : send topic subject
		channel_name = arguments;
		result = _server->searchChannel(channel_name);
		if (!result.first || result.second->isMode('s'))
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
		else if(!result.second->isUser(client.getNickname()))
			client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel_name));
		else if (result.second->getTopic().empty())
			client.reply(RPL_NOTOPIC(client.getNickname(), channel_name));
		else
			client.reply(RPL_TOPIC(client.getNickname(), channel_name, result.second->getTopic()));
	}
}
