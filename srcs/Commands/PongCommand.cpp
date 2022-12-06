#include "../../inc/Command.hpp"

PongCommand::PongCommand(Server *server) : Command(server) {}

PongCommand::~PongCommand() {}

void PongCommand::execute(Client& client, std::string arguments) {

	if (arguments.empty()) {
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PONG"));
		return;
	}
}