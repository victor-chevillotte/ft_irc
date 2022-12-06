#include "../../inc/Command.hpp"

NamesCommand::NamesCommand(Server *server) : Command(server) {}

NamesCommand::~NamesCommand() {}

void NamesCommand::execute(Client &client, std::string arguments)
{

  // no argument : lists all channel and their users
  if (arguments.empty())
  {
    for (std::vector<Channel>::iterator it = _server->getChannelBegin(); it != _server->getChannelEnd(); ++it)
    {
      if (!it->isMode('s') || it->isUser(client.getNickname()))
        it->sendUserList(client, false);
    }
    _server->sendUnjoinedUserList(client);
    return;
  }

  Channel *channel = _server->getChannel(arguments);
  if (!channel)
  {
    client.reply(RPL_ENDOFNAMES(client.getNickname(), arguments));
  }

  // checks if channel's 'secret mode' is enabled and client not on channel
  if (!channel->isUser(client.getNickname()) && channel->isMode('s'))
    client.reply(RPL_ENDOFNAMES(client.getNickname(), arguments));

  // reply userlist without invisible users
  else if (!channel->isUser(client.getNickname()))
  {
    client.reply(RPL_NAMEREPLY(client.getNickname(), channel->getSymbol(), channel->getName(), channel->getUserList(false)));
  }

  // reply full userlist
  else
    client.reply(RPL_NAMEREPLY(client.getNickname(), channel->getSymbol(), channel->getName(), channel->getUserList(true)));
  client.reply(RPL_ENDOFNAMES(client.getNickname(), arguments));
}
