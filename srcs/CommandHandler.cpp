#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#include "../inc/CommandHandler.hpp"

CommandHandler::CommandHandler(Server *server) : _server(server)
{
	_commands["NICK"] = new NickCommand(_server, false);
	_commands["USER"] = new UserCommand(_server, false);
	_commands["PASS"] = new PassCommand(_server, false);
	_commands["QUIT"] = new QuitCommand(_server, false);

	_commands["CAP"] = new CapCommand(_server);
	_commands["NAMES"] = new NamesCommand(_server);
	_commands["LIST"] = new ListCommand(_server);

	_commands["PING"] = new PingCommand(_server);
	_commands["PONG"] = new PongCommand(_server);

	_commands["JOIN"] = new JoinCommand(_server);
	_commands["MODE"] = new ModeCommand(_server);
	_commands["PART"] = new PartCommand(_server);
	_commands["TOPIC"] = new TopicCommand(_server);
	_commands["KICK"] = new KickCommand(_server);
	_commands["PRIVMSG"] = new PrivMsgCommand(_server);
	_commands["NOTICE"] = new NoticeCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}

void CommandHandler::parseExecute(Client &client, std::string message)
{
	// split of the first word of message to get command
	std::vector<std::string> arguments;
	splitCommand(arguments, message);

	client.setLastPingTime(time(NULL));
	try
	{
		Command *command = _commands.at(arguments[0]);

		if (!client.isAuthentified() && command->authRequired())
		{
			std::string nick = client.getNickname().empty() ? "default_nickname" : client.getNickname();
			client.reply(ERR_NOTAUTHENTIFIED(nick));
			return;
		}

		command->execute(client, arguments[1]);
	}
	catch (const std::out_of_range &e)
	{
		std::cout << "Command unknown :" << std::endl;
		std::cout << message << std::endl;
		client.reply(ERR_UNKNOWNCOMMAND(client.getNickname(), message));
	}
}

void CommandHandler::splitCommand(std::vector<std::string> &arguments, const std::string &message)
{
	size_t pos = 0;

	pos = message.find(" ");
	if (pos < message.size())
	{
		arguments.push_back(message.substr(0, pos));
		arguments.push_back(message.substr(pos + 1));
	}
	else
		arguments.push_back(message);
}