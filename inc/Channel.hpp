#pragma once

#include <vector>
#include <string>
#include "Client.hpp"
#include <sstream>

class Client;

class Channel
{
	private:

		std::vector<Client>	_user_list;
		std::vector<Client>	_op_list;
		std::string			_name;
		std::string			_topic;
		std::string			_modes;
		std::string			_ban_list;
		size_t				_user_nb;
		std::vector<std::string> _user_invited_list;
		std::vector<std::string> _user_banned_list;
		std::vector<std::string> _user_excepted_from_ban_list;
		std::vector<std::string> _user_excepted_from_invited_list;
		size_t				_user_limit;
		std::string			_symbol;
		std::string			_password;

	public:

		Channel(void);
		Channel(const std::string& chan_name);
		Channel(const Channel& other);
		~Channel(void);

		Channel& operator=(const Channel& rhs);

		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getModes() const;
		size_t		getUserNb() const;
		std::string	getUserNbStr() const;
		std::string	getSymbol() const;
		Client&		getChanClient(const std::string& client_name);
		void		setName(const std::string new_name);
		void		setTopic(const std::string new_topic);
		void		setModes(const std::string new_modes);
		void		setSymbol(const std::string& new_symbol);
		bool		isUser(const std::string nick) const;
		bool		isOp(const std::string nick) const;
		bool		isMode(char mode) const;
		bool		isClientMode(const std::string client, char mode) const;
		bool		addMode(char mode);
		bool		removeMode(char mode);
		void		addUser(Client user);
		void		addOp(Client op);
		void		delUser(Client user);
		void		delOp(Client op);
		std::string getUserList(bool show_invisible);
		void		sendUserList(Client& client, bool show_invisible);
		void		broadcastMessage(std::string message);
		void		broadcastExceptSource(std::string message, const std::string& source_nick);
		bool		isInvited(const std::string nickname) const;
		void		addInvitation(const std::string nickname);
		void		removeInvitation(const std::string nickname);
		bool		isBanned(const std::string nickname) const;
		void		addBan(Client& client, const std::string nickname);
		void		removeBan(const std::string nickname);
		bool		hasReachedClientsLimit() const;
		bool		isPassword(const std::string password) const;
		void		setChannelLimit(const size_t new_channel_limit);
		void		setPassword(const std::string new_password);
		void		addExceptionBan(const std::string nickname);
		void		removeExceptionBan(const std::string nickname);
		bool		isExceptedFromBan(const std::string nickname) const;
		bool		isExceptedFromInvite(const std::string nickname) const;
		void		addExceptionInvite(const std::string nickname);
		void		removeExceptionInvite(const std::string nickname);


};