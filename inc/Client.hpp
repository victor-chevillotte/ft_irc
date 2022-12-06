#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h> // socket()
#include "utils.hpp"
#include <ctime>

#define MSG_SIZE_LIMIT 500

class Client
{
	public:
	
		Client(void);

		Client(int fd, const std::string hostname, int port, const std::string serverPrefix);

		Client(const Client& other);
		
		~Client(void);

		Client&	operator=(const Client& other);

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getRealName() const;
		const std::string&	getPassword() const;
		const std::string&	getHostname() const;
		const std::string&	getModes() const;
		const std::string&	getServerPrefix() const;
		const std::string	getChanPrefix() const;
		Client*				getClientOnServer();
		bool 				isAuthentified() const;
		bool 				isRegistered() const;
		bool				isMode(char mode) const;
		int					getSocketfd() const;
		int					getPort() const;
		std::string 		getPrefix() const;
		std::string&		getMessageBuffer();
		std::string&		getSendQueue();
		time_t				getLastPingTime();
		int&				getJoinedChannelNb();
		void				setLastPingTime(time_t new_lastPingTime);
		void				addSendQueue(const std::string message);
		void 				write(const std::string &message) const;
		void				reply(const std::string &reply);
		void 				welcome();
		std::string			extractMessage();
		void				setIsAuthentified(bool is_Authentified);
		void				setIsRegistered(bool is_Registered);
		void				setNickname(const std::string& new_nickname);
		void				setRealName(const std::string& new_nickname);
		void				setUsername(const std::string& new_username);
		void				setPassword(const std::string& new_password);
		void				setPtr(Client* client);
		bool				addUserMode(char mode);
		bool				removeUserMode(char mode);
		void				resetUserModes();

	private:

		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _realName;
		std::string _password;
		std::string	_hostname;
		bool 		_isAuthentified;
		bool 		_isRegistered;
		int 		_port;
		int			_joinedChannels; // server only
		std::string	_serverPrefix;
		std::string _modes; // channel only
		std::string _messageBuffer; // server only
		time_t		_lastPingTime; // server only
		std::string	_sendQueue; // server only
		Client*		_clientOnServer;

};