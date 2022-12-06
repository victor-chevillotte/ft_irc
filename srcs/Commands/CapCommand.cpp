#include "../../inc/Command.hpp"

CapCommand::CapCommand(Server *server) : Command(server) {}

CapCommand::~CapCommand() {}

void CapCommand::execute(Client &client, std::string arguments)
{

  (void)arguments;
  (void)client;
  std::cout << "The server does not support CAP negociation." << std::endl;
}
