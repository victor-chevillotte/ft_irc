#include "../../inc/Command.hpp"

NickCommand::NickCommand(Server *server, bool auth) : Command(server, auth) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client& client, std::string arguments) {
	if (arguments.empty()) {
		client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()));
		return;
	}

	if (_server->getClient(arguments)) {
		client.reply(ERR_NICKNAMEALREADYUSED(arguments));
		return;
	}
	//Check non conform Nicknames
	if (arguments.find(' ') != std::string::npos || arguments.find('.') != std::string::npos || arguments.find(',') != std::string::npos 
	|| arguments.find('*') != std::string::npos || arguments.find('?') != std::string::npos || arguments.find('!') != std::string::npos 
	|| arguments.find('@') != std::string::npos || arguments[0] == '$' || arguments[0] == ':' || arguments[0] == '+' || arguments[0] == '=' || !arguments.compare("anonymous") )
	{
		client.reply(ERR_ERRONEUSNICKNAME(client.getNickname(), arguments));
		return;
	}
	_server->allChannelBroadcast(client.getNickname(), CHANGEDNICKNAME(client.getPrefix(), arguments));
	_server->allChannelChangeNickname(client.getNickname(), arguments);
	client.setNickname(arguments);
	std::cout << "Client Nickname set : " << client.getNickname() << std::endl;
	client.welcome();
}