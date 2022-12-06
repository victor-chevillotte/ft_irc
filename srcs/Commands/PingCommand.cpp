#include "../../inc/Command.hpp"

PingCommand::PingCommand(Server *server) : Command(server) {}

PingCommand::~PingCommand() {}

void PingCommand::execute(Client& client, std::string arguments) {

	if (arguments.empty()) {
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PING"));
		return;
	}
	client.write(RPL_PONG(_server->getServerPrefix(), arguments));
}