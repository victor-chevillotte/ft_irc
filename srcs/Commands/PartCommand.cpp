#include "../../inc/Command.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client& client, std::string arguments) {

  // arguments parseExecute to extract channel_name
  size_t pos = arguments.find(" ");
  std::string channel_name = arguments.substr(0, pos);

  // check if channel exists and proceed
  std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(channel_name);
  if (result.first)
	{
    // check if user is not on channel
    if (!result.second->isUser(client.getNickname()))
    {
      client.reply(ERR_NOTONCHANNEL(client.getNickname(), result.second->getName()));
      return ;
    }
    // delete user and delete channel if last user
    std::string prefix = client.getPrefix();
    if (!result.second->isMode('q'))
      result.second->broadcastMessage(":" + prefix + " PART " + arguments);
    client.getJoinedChannelNb()--;
    result.second->delUser(client);
    if (result.second->getUserList(true).empty())
      _server->removeChannel(result.second);
	}
  else
    client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
}
