#pragma once

class CommandHandler;
class Command;
class Channel;

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"

class Server;
class CommandHandler
{

private:
	Server *_server;
	std::map<std::string, Command *> _commands;

public:

	typedef std::map<std::string, Command *>::iterator 		commands_iterator;
	typedef std::vector<Channel>::iterator					channel_iterator;
	typedef std::pair<bool, std::vector<Channel>::iterator> channel_search_res;

	
	CommandHandler(Server *server);

	~CommandHandler();

	void parseExecute(Client& client, std::string message);
	void splitCommand(std::vector<std::string> &arguments, const std::string& message);
};
