#include "../../inc/Command.hpp"

PassCommand::PassCommand(Server *server, bool auth) : Command(server, auth) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client &client, std::string arguments)
{

	if (client.isAuthentified())
	{
		client.reply(ERR_ALREADYAUTHENTIFIED(client.getNickname()));
		return;
	}

	if (arguments.empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PASS"));
		return;
	}

	if (_server->getPassword() != arguments.substr(arguments[0] == ':' ? 1 : 0))
	{
		std::string nick = client.getNickname().empty() ? "default_nickname" : client.getNickname();
		client.reply(ERR_WRONGPASSWORD(nick));
		return;
	}

	client.setIsAuthentified(true);
}