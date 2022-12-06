#include "../inc/utils.hpp"
#include "../inc/IrcBot.hpp"

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result)
{
	std::string message = str;
	size_t		pos = 0;

	while (message.size() != 0)
	{
		pos = message.find(sep);
		if (pos > message.size())
		{
			result.push_back(message);
			break;
		}
		result.push_back(message.substr(0, pos));
		message.erase(0, pos + sep.size());
	}
}

void	sig_quit(int)
{
	std::cout << "Bot is being turned down. Please wait." << std::endl;
	IrcBot::running = false;
}