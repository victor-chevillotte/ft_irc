#include "../inc/Client.hpp"

Client::Client() {

}

Client::Client(int fd, const std::string hostname, int port, const std::string serverPrefix)
		:	_socketfd(fd),
			_hostname(hostname),
			_isAuthentified(false),
			_isRegistered(false),
			_port(port),
			_joinedChannels(0),
			_serverPrefix(serverPrefix),
			_clientOnServer(this) {
	
	_lastPingTime = time(NULL);
}

Client::Client(const Client& other) :	_socketfd(other.getSocketfd()), 
										_nickname(other.getNickname()),
										_username(other.getUsername()),
										_realName(other.getRealName()),
										_password(other.getPassword()),
										_hostname(other._hostname),
										_isAuthentified(other.isAuthentified()),
										_isRegistered(other.isRegistered()),
										_port(other.getPort()),
										_joinedChannels(other._joinedChannels),
										_serverPrefix(other.getServerPrefix()),
										_modes(other.getModes()),
										_clientOnServer(other._clientOnServer) {

	_lastPingTime = time(NULL);
	
}

Client::~Client() {
}

Client&	Client::operator=(const Client& rhs) {
	_socketfd = rhs.getSocketfd();
	_nickname = rhs.getNickname();
	_username = rhs.getUsername();
	_realName = rhs.getRealName();
	_password = rhs.getPassword();
	_isAuthentified = rhs.isAuthentified();
	_isRegistered = rhs.isRegistered();
	_hostname = rhs._hostname ;
	_port = rhs.getPort();
	_joinedChannels = rhs._joinedChannels;
	_serverPrefix = rhs.getServerPrefix();
	_modes = rhs.getModes();
	_clientOnServer = rhs._clientOnServer;
	_lastPingTime = rhs._lastPingTime;

	return *this ;
}

std::string Client::getPrefix() const {
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

void Client::write(const std::string &message) const {
	_clientOnServer->addSendQueue(message);
}

void Client::reply(const std::string &reply) {
	_clientOnServer->addSendQueue(":" + _serverPrefix + " " + reply);
}

void Client::welcome() {
	if (_isAuthentified == false || _username.empty() || _realName.empty() || _nickname.empty())
		return;
	if (_isRegistered == true)
	{
		reply(RPL_CHANGENICK(_nickname));
		std::cout << "Nickname changed to "  << _nickname << std::endl;
		return;
	}
	setIsRegistered(true);
	reply(RPL_WELCOME(_nickname));
	std::cout << "welcome messsage sent to "  << _nickname << std::endl;
}

bool	Client::isAuthentified() const {
	return _isAuthentified;
}

bool	Client::isRegistered() const {
	return _isRegistered;
}

bool	Client::isMode(char mode) const {
	return (getModes().find(mode) < getModes().size());
}


const std::string&	Client::getNickname() const {
	return _nickname;
}

const std::string&	Client::getUsername() const {
	return _username;
}

const std::string&	Client::getRealName() const {
	return _realName;
}

const std::string&	Client::getPassword() const {
	return _password;
}

int					Client::getSocketfd() const {
	return _socketfd;
}

int					Client::getPort() const {
	return _port;
}

const std::string&	Client::getHostname() const {
	return _hostname;
}

const std::string&	Client::getModes() const {
	return _modes;
}

const std::string&	Client::getServerPrefix() const {
	return _serverPrefix;
}

const std::string	Client::getChanPrefix() const {
	if (isMode('@'))
		return ("@");
	if (isMode('+'))
		return ("+");
	return ("");
}

time_t		Client::getLastPingTime() {
	return _lastPingTime;
}

std::string&		Client::getMessageBuffer() {
	return _messageBuffer;
}

std::string&		Client::getSendQueue() {
	return _sendQueue;
}

Client*				Client::getClientOnServer() {
	return _clientOnServer;
}



void				Client::setNickname(const std::string& new_nickname) {
	_nickname = new_nickname;
}

void				Client::setUsername(const std::string& new_username) {
	_username = new_username;
}

void				Client::setRealName(const std::string& new_realName) {
	_realName = new_realName;
}

void				Client::setPassword(const std::string& new_password) {
	_password = new_password;
}

void				Client::setIsAuthentified(bool is_Authentified) {
	_isAuthentified = is_Authentified;
}

void				Client::setIsRegistered(bool is_Registered) {
	_isRegistered = is_Registered;
}

void				Client::setLastPingTime(time_t new_lastPingTime) {
	_lastPingTime = new_lastPingTime;
}

void				Client::setPtr(Client* clientPtr) {
	_clientOnServer = clientPtr;
}

std::string			Client::extractMessage() {

	std::string message; 

	size_t pos = _messageBuffer.find("\r\n");
	message = _messageBuffer.substr(0, pos);
	_messageBuffer.erase(0, pos + 2);

	std::cout << "extracted message :" + message << std::endl;
	return message;
}

void				Client::addSendQueue(const std::string message) {
	
	std::string full_message = message;
	if (full_message.size() > MSG_SIZE_LIMIT)
		full_message.resize(MSG_SIZE_LIMIT);
	full_message += "\r\n";
	_sendQueue += full_message;
}

bool				Client::addUserMode(char mode) {
	if (isMode(mode))
		return false;
	_modes += mode;
	return true;
}

bool				Client::removeUserMode(char mode) {
	if (!isMode(mode))
		return false;
	size_t pos = _modes.find(mode);
	_modes.erase(pos, 1);
	return true;
}

void				Client::resetUserModes() {
	_modes = "";
}


int&				Client::getJoinedChannelNb() {
	return _joinedChannels;
}