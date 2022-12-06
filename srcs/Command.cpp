#include "../inc/Command.hpp"

Command::Command(Server *server, bool authRequired) : _server(server), _authRequired(authRequired)
{};

Command::~Command()
{};

bool Command::authRequired() const {
	return _authRequired;
};