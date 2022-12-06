#include "../inc/Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(const std::string &chan_name) : _name(chan_name), _topic(""), _modes("+nt"), _user_nb(0), _symbol("=")
{
}

Channel::Channel(const Channel &other) : _user_list(other._user_list),
										 _op_list(other._op_list),
										 _name(other._name),
										 _topic(other._topic),
										 _modes(other._modes),
										 _user_nb(other._user_nb),
										 _symbol(other._symbol)
{
}

Channel::~Channel(void)
{
}

Channel &Channel::operator=(const Channel &rhs)
{
	_user_list = rhs._user_list;
	_op_list = rhs._op_list;
	_name = rhs._name;
	_topic = rhs._topic;
	_modes = rhs._modes;
	_user_nb = rhs._user_nb;
	_symbol = rhs._symbol;

	return *this;
}

std::string Channel::getName() const
{
	return _name;
}

std::string Channel::getTopic() const
{
	return _topic;
}

std::string Channel::getModes() const
{
	return _modes;
}

size_t Channel::getUserNb() const
{
	return _user_nb;
}

std::string Channel::getUserNbStr() const
{
	std::string result;
	std::stringstream out;

	out << _user_nb;
	result = out.str();
	return std::string(result);
}

std::string Channel::getSymbol() const
{
	return _symbol;
}

Client &Channel::getChanClient(const std::string &client_name)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == client_name)
			return *it;
	}
	throw std::out_of_range("client not found");
}

void Channel::setName(const std::string new_name)
{
	_name = new_name;
}

void Channel::setTopic(const std::string new_topic)
{
	_topic = new_topic;
}

void Channel::setModes(const std::string new_modes)
{
	_modes = new_modes;
}

void Channel::setChannelLimit(const size_t new_channel_limit)
{
	_user_limit = new_channel_limit;
}

void Channel::setSymbol(const std::string &new_symbol)
{
	_symbol = new_symbol;
}

void Channel::setPassword(const std::string new_password)
{
	_password = new_password;
}

bool Channel::addMode(char mode)
{
	if (isMode(mode))
		return false;
	_modes += mode;
	return true;
}

bool Channel::removeMode(char mode)
{
	if (!isMode(mode))
		return false;
	size_t pos = _modes.find(mode);
	_modes.erase(pos);
	return true;
}

bool Channel::isUser(const std::string nick) const
{
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == nick)
			return true;
	}
	return false;
}

bool Channel::isPassword(const std::string password) const
{
	return (password == _password);
}

bool Channel::isOp(const std::string op) const
{
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == op)
			return (it->isMode('@'));
	}
	return false;
}

bool Channel::isClientMode(const std::string nick, char mode) const
{
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == nick)
			return (it->isMode(mode));
	}
	return false;
}

bool Channel::isInvited(const std::string nickname) const
{
	for (std::vector<std::string>::const_iterator it = _user_invited_list.begin(); it != _user_invited_list.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void Channel::addInvitation(const std::string nickname)
{
	_user_invited_list.push_back(nickname);
}

void Channel::removeInvitation(const std::string nickname)
{
	for (std::vector<std::string>::const_iterator it = _user_invited_list.begin(); it != _user_invited_list.end(); ++it)
	{
		if (*it == nickname)
		{
			_user_invited_list.erase(it);
			return;
		}
	}
	Client to_kick_user;
	if (isUser(nickname) && !isInvited(nickname))
	{
		to_kick_user = getChanClient(nickname);
		delUser(to_kick_user);
		_user_nb--;
	}
}

bool Channel::isMode(char mode) const
{
	return (getModes().find(mode) < getModes().size());
}

bool Channel::isBanned(const std::string nickname) const
{
	for (std::vector<std::string>::const_iterator it = _user_banned_list.begin(); it != _user_banned_list.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

bool Channel::isExceptedFromInvite(const std::string nickname) const
{
	for (std::vector<std::string>::const_iterator it = _user_excepted_from_invited_list.begin(); it != _user_excepted_from_invited_list.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void Channel::addExceptionInvite(const std::string nickname)
{
	_user_excepted_from_invited_list.push_back(nickname);
}

void Channel::removeExceptionInvite(const std::string nickname)
{
	for (std::vector<std::string>::const_iterator it = _user_excepted_from_invited_list.begin(); it != _user_excepted_from_invited_list.end(); ++it)
	{
		if (*it == nickname)
		{
			_user_excepted_from_invited_list.erase(it);
			return;
		}
	}
	Client to_kick_user;
	if (isUser(nickname) && !isInvited(nickname))
	{
		to_kick_user = getChanClient(nickname);
		delUser(to_kick_user);
		_user_nb--;
	}
}

bool Channel::isExceptedFromBan(const std::string nickname) const
{
	for (std::vector<std::string>::const_iterator it = _user_excepted_from_ban_list.begin(); it != _user_excepted_from_ban_list.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void Channel::addExceptionBan(const std::string nickname)
{
	_user_excepted_from_ban_list.push_back(nickname);
}

void Channel::removeExceptionBan(const std::string nickname)
{
	for (std::vector<std::string>::const_iterator it = _user_excepted_from_ban_list.begin(); it != _user_excepted_from_ban_list.end(); ++it)
	{
		if (*it == nickname)
		{
			_user_excepted_from_ban_list.erase(it);
			return;
		}
	}
	Client to_kick_user;
	if (isUser(nickname) && isBanned(nickname))
	{
		to_kick_user = getChanClient(nickname);
		std::string prefix = isMode('a') ? "anonymous!anonymous@anonymous." : to_kick_user.getPrefix();
		broadcastMessage(":" + prefix + " KICK " + to_kick_user.getNickname());
		delUser(to_kick_user);
		_user_nb--;
	}
}

void Channel::addBan(Client& client, const std::string nickname)
{
	Client kicked_user;
	if (isUser(nickname))
	{
		kicked_user = getChanClient(nickname);
		std::string prefix = isMode('a') ? "anonymous!anonymous@anonymous." : client.getPrefix();
		broadcastMessage(":" + prefix + " KICK " + getName() + " " + kicked_user.getNickname() + " :banned from channel");
		delUser(kicked_user);
		_user_nb--;
	}
	_user_banned_list.push_back(nickname);
}

void Channel::removeBan(const std::string nickname)
{
	for (std::vector<std::string>::const_iterator it = _user_banned_list.begin(); it != _user_banned_list.end(); ++it)
	{
		if (*it == nickname)
		{
			_user_banned_list.erase(it);
			return;
		}
	}
}

bool Channel::hasReachedClientsLimit() const
{
	return (_user_nb >= _user_limit);
}

void Channel::addUser(Client user)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
			return;
	}
	_user_list.push_back(user);
	_user_nb++;
}

void Channel::delUser(Client user)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
		{
			_user_list.erase(it);
			_user_nb--;
			return;
		}
	}
	// Error message if user is not on channel
	user.reply(ERR_NOTONCHANNEL(user.getNickname(), this->getName()));
}

std::string Channel::getUserList(bool show_invisible)
{
	std::string result;

	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (!show_invisible && it->getClientOnServer()->isMode('i'))
			continue;
		result += " ";
		result += it->getChanPrefix();
		result += it->getNickname();
	}
	return result;
}

void Channel::sendUserList(Client &client, bool show_invisible)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (!show_invisible && it->getClientOnServer()->isMode('i'))
			continue;
		client.reply(RPL_NAMEREPLY(client.getNickname(), getSymbol(), getName(), it->getChanPrefix() + it->getNickname()));
	}
	client.reply(RPL_ENDOFNAMES(client.getNickname(), getName()));
}

void Channel::broadcastMessage(std::string message)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		it->write(message);
	}
}

void Channel::broadcastExceptSource(std::string message, const std::string &source_nick)
{
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() != source_nick)
			it->write(message);
	}
}
