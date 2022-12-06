#include "../inc/Server.hpp"
#include "../inc/CommandHandler.hpp"

Server::Server(const std::string port, const std::string password)
	: _host("127.0.0.1"), _name("ft_irc"), _port(port), _password(password), _commandHandler(this)
{
	_sock = newSocket();
}

Server::~Server()
{
}

void Server::start()
{
	time_t lastPingTime = time(NULL);
	pollfd server_fd = {_sock, POLLIN, 0}; // POLLHUP & POLLERR sont fournis automatiquement
	_pollfds.push_back(server_fd);

	// Le server écoute désormais les POLL IN
	while (Server::running)
	{
		time_t actualTime = time(NULL);
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			// ping all clients at interval PING_INTERVAL
			if (actualTime > (lastPingTime + PING_INTERVAL))
				it->second.addSendQueue(RPL_PING(getServerPrefix(), std::string("check if client is still connected")));
			// Check that clients have answered to ping
			if (it->second.getLastPingTime() + PING_INTERVAL + TIMEOUT < actualTime)
			{
				it->second.write(RPL_QUIT(it->second.getPrefix(), "Can't reach user : timeout"));
				allChannelLeave(it->second, RPL_QUIT(it->second.getPrefix(), "Client has been kicked because he did not reply to Ping"));
				std::cout << "Client has Timeout " << std::endl;
				_fdToDelete.push_back(it->second.getSocketfd());
			}
		}

		if (actualTime > (lastPingTime + PING_INTERVAL))
			lastPingTime = actualTime;
			
		// poll est une fonction qui boucle jusqu'à l'arrivée de nouvelles data
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			if (Server::running)
				throw std::runtime_error("Error while polling from fd.");
		//  Un des fd a un nouveau message, on les parcourt pour savoir lequel
		for (pollfds_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{

			if (it->revents & POLLHUP)
			{
				onClientDisconnect(it->fd);
				break;
			}

			if (it->revents & POLLIN)
			{

				if (it->fd == _sock)
				{
					onClientConnect();
					break;
				}
				else
					onClientMessage(it->fd);
			}

			// POLLOUT for Clients
			if ((it->fd != _sock) && (it->revents & POLLOUT))
			{
				sendMessage(_clients[it->fd]);
			}

			// POLLERR for Clients : allChannelLeave + deleteClient
			if ((it->fd != _sock) && (it->revents & POLLERR))
			{
				onClientDisconnect(it->fd);
			}

			//server POLLERR -> Server::running = false
			if ((it->fd == _sock) && (it->revents & POLLERR))
			{
				Server::running = false;
			}

		}
		for (std::vector<int>::iterator it = _fdToDelete.begin(); it != _fdToDelete.end(); ++it)
		{
			deleteClient(*it);
			std::cout << "Client deleted" << std::endl;
		}
		_fdToDelete.clear();
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		close(it->second.getSocketfd());
	}
	close(_sock);
	std::cout << "Server has been turned down. Goodbye !" << std::endl;
}

int Server::newSocket()
{

	/* creating socket :
	 * domain : AF_INET -> Socket using IPV4
	 * type : SOCK_STREAM : Dialogue support guaranteeing integrity, providing a binary data stream, and integrating a mechanism for out-of-band data transmissions.
	 * protocol : 0 indicates that the caller does not want to specify the protocol and will leave it up to the service provider.
	 */
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");

	/*Adding option to socket :
	 * Socket Layer : SOL_SOCKET : Means we modify the socket itslef
	 * option : SO_REUSEADDR : Forcefully attaching socket to the port
	 * value : 1 for forcing socket to use port given
	 */
	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");

	/* Setting the socket to NON-BLOCKING mode allowing it to return any data that the system has in it's read buffer
	 * for that socket even if the fd is still in use. It won't wait for that data to be terminated and will send an error.
	 * command : F_SETFL : setting for state attribute of fd
	 * arg : O_NONBLOCK meaning described previously
	 */
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	}

	// Creating serv_address, giving the parameters to the struct then biding it to the socket
	struct sockaddr_in serv_address = {};

	// Clear address structure
	bzero((char *)&serv_address, sizeof(serv_address));

	serv_address.sin_family = AF_INET;				 // Socket using IPV4
	serv_address.sin_addr.s_addr = INADDR_ANY;		 // means can attached socket to any address
	serv_address.sin_port = htons(std::stoi(_port)); // TCP protocol does not read a port int so we use htons() to convert unsigned short int to big-endian network byte order as expected from TCP protocol standards

	// Bind the socket to the IP address and port
	if (bind(sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0)
		throw std::runtime_error("Error while setting socket IP address and port.");

	// Define max connexions and let socket be able to listen for requests
	if (listen(sockfd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");

	return sockfd;
}

void Server::onClientConnect()
{

	// adding new fd to poll
	int fd;
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	fd = accept(_sock, (sockaddr *)&s_address, &s_size);
	if (fd < 0)
		throw std::runtime_error("Error while accepting new client.");

	pollfd pollfd = {fd, POLLIN | POLLOUT, 0};
	_pollfds.push_back(pollfd);

	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname of new client.");

	// Checks hostname size limit of 63 chars
	std::string host(hostname);
	if (host.length() > 63)
		host = inet_ntoa(s_address.sin_addr);

	// Creates a new Client and store it in Clients map
	_clients.insert(std::make_pair(fd, Client(fd, host, ntohs(s_address.sin_port), getServerPrefix())));
	_clients[fd].setPtr(&_clients[fd]);
	std::cout << "Client connnected" << std::endl;
}

void Server::onClientDisconnect(int fd)
{

	// client is removed from all subscribed channels
	allChannelLeave(_clients[fd], ":" + _clients[fd].getPrefix() + " QUIT :Quit:");

	// removing fd of leaving client from poll
	deleteClient(fd);
	std::cout << "Client has disconnected" << std::endl;
}

void Server::onClientMessage(int fd)
{
	readMessage(fd);
}

void Server::readMessage(int fd)
{

	int read_bytes = -10;
	char buffer[BUFFER_SIZE + 1];

	bzero(buffer, BUFFER_SIZE + 1);
	while (read_bytes != 0)
	{
		bzero(buffer, BUFFER_SIZE);
		read_bytes = recv(fd, buffer, BUFFER_SIZE, 0);
		if (read_bytes < 0)
			break;
		buffer[read_bytes] = '\0';
		_clients[fd].getMessageBuffer().append(buffer);
		while (_clients[fd].getMessageBuffer().find("\r\n") < _clients[fd].getMessageBuffer().size())
		{
			_commandHandler.parseExecute(_clients[fd], _clients[fd].extractMessage());
		}
	}
}

void Server::sendMessage(Client &client)
{

	if (client.getSendQueue().empty())
		return;
	int sent_bytes = send(client.getSocketfd(), client.getSendQueue().c_str(), client.getSendQueue().length(), 0);
	if (sent_bytes < 0)
		std::cout << "Error while sending message to client." << std::endl;
	std::cout << "message sent :" + client.getSendQueue().substr(0, sent_bytes);
	client.getSendQueue().erase(0, sent_bytes);
}

Client *Server::getClient(const std::string nickname)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (!nickname.compare(it->second.getNickname()))
			return &it->second;
	}
	return nullptr;
}

Client *Server::getClientByUsername(const std::string username)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (!username.compare(it->second.getUsername()))
			return &it->second;
	}
	return nullptr;
}

Channel* Server::getChannel(const std::string &channel_name)
{
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->getName() == channel_name)
			return &(*it);
	}
	return nullptr;
}

void Server::deleteClient(int fd)
{
	_clients.erase(fd);
	
	for (pollfds_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_pollfds.erase(it);
			close(fd);
			break;
		}
	}
}

void Server::addChannel(Channel channel)
{
	_channels.push_back(channel);
}

void Server::removeChannel(std::vector<Channel>::iterator pos)
{
	_channels.erase(pos);
}

std::pair<bool, std::vector<Channel>::iterator> Server::searchChannel(const std::string channel_name)
{

	std::vector<Channel>::iterator iter = _channels.begin();

	for (; iter != _channels.end(); ++iter)
	{

		if (iter->getName() == channel_name)
		{
			return std::make_pair(true, iter);
		}
	}
	return std::make_pair(false, iter);
}

void Server::allChannelLeave(Client& client, std::string broadcast_message)
{

	std::vector<std::string>	channels_to_remove;
	std::string					message = broadcast_message;

	for (channel_iterator chan_iter = _channels.begin(); chan_iter != _channels.end(); ++chan_iter)
	{
		if (chan_iter->isUser(client.getNickname()))
		{
			chan_iter->delUser(client);
			if (chan_iter->getUserList(true).empty())
				channels_to_remove.push_back(chan_iter->getName());
			if (broadcast_message.empty() || chan_iter->isMode('a'))
				message = ":" + client.getPrefix() + " PART " + chan_iter->getName();
			chan_iter->broadcastMessage(message);
		}
	}
	for (std::vector<std::string>::iterator iter = channels_to_remove.begin(); iter != channels_to_remove.end(); ++iter)
	{
		removeChannel(this->searchChannel(*iter).second);
	}
	client.getJoinedChannelNb() = 0;
}

void Server::allChannelBroadcast(const std::string& nick, const std::string& message)
{
	for (channel_iterator chan_iter = _channels.begin(); chan_iter != _channels.end(); ++chan_iter)
	{
		if (chan_iter->isUser(nick) && !chan_iter->isMode('q'))
		{
			chan_iter->broadcastMessage(message);
		}
	}
}

void Server::allChannelChangeNickname(const std::string& old_nick, const std::string& new_nick)
{
	for (channel_iterator chan_iter = _channels.begin(); chan_iter != _channels.end(); ++chan_iter)
	{
		if (chan_iter->isUser(old_nick))
		{
			Client& client = chan_iter->getChanClient(old_nick);
			client.setNickname(new_nick);
		}
	}
}

void Server::addClientToDelete(int fd) {
	_fdToDelete.push_back(fd);
}

std::string Server::getServerPrefix() const {
	return (_name + "@" + _host);
}

std::vector<Channel>::iterator	Server::getChannelBegin() {
	return _channels.begin();
}

std::vector<Channel>::iterator	Server::getChannelEnd() {
	return _channels.end();
}

void							Server::sendUnjoinedUserList(Client& client) {

	for (clients_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getJoinedChannelNb() == 0 && !it->second.getClientOnServer()->isMode('i'))
		{
			client.reply(RPL_NAMEREPLY(client.getNickname(), std::string("*"), std::string("channel"), it->second.getNickname()));
		}
	}
	client.reply(RPL_ENDOFNAMES(client.getNickname(), std::string("channel")));
}

std::string	Server::getPassword() const {
	return _password;
}

bool Server::running = true;