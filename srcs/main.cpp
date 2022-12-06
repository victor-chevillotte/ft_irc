#include "../inc/Server.hpp"
#include "../inc/utils.hpp"

int main(int ac, char **av)
{
	signal(SIGINT, &sig_quit);
	try
	{
		if (ac != 3)
		{
			throw std::runtime_error("Usage: ./ircserv <port> <password>");
		}
		Server server(av[1], av[2]);
		server.start();
		return 0;
	}
	catch (const std::exception &except)
	{
		std::cerr << except.what() << std::endl;
		return 1;
	}
}