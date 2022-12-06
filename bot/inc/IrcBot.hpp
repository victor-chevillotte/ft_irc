#pragma once


#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h> // socket()
#include <netinet/in.h> // sockaddr_in
#include <utility> // std::make_pair
#include <unistd.h> // close
#include <arpa/inet.h> /* htons et inet_aton */
#include "utils.hpp"

#define MSG_SIZE_LIMIT 450
#define BUFFER_SIZE 10000

class IrcBot {
public:
	IrcBot(const std::string &host, const std::string &port, const std::string &password);

	~IrcBot();

	void start();

	static bool running;

private:

	typedef std::vector<pollfd>::iterator 	pollfds_iterator;

	int 				_sock;
	std::string 		_host;
	std::string 		_port;
	std::string 		_password;
	std::string 		_buffer;
	std::vector<pollfd>	_pollfds;
	std::string			_messageBuffer;
	std::string			_sendQueue;
	std::string			_serverPrefix;


	int			newSocket();
	void 		addSendQueue(const std::string& message);
	void 		sendMessageToServer();
	void 		sendPrivMsg(const std::string &source, const std::string &message);
	void 		authenticate(const std::string &nickname);
	void 		onMessageReceived(const std::string &message);
	std::string	extractMessage();
	void 		onServerMessage(int fd);
	void		ParseCommand(std::vector<std::string>& command, std::string message);
	void		parseExecute(const std::string& message);
	void		rockPaperScissors(const std::vector<std::string>& command);
	std::string	whoWins(const std::string& userChoice, const std::string& botChoice);

};
