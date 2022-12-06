#include "../inc/IrcBot.hpp"

int main(int argc, char **argv) {

	signal(SIGINT, &sig_quit);
	try {
		if (argc < 3) {
			throw std::runtime_error("Usage: ./ircbot <host> <port> [password]");
		}
		IrcBot bot(argv[1], argv[2], argc == 4 ? argv[3] : "");
		
		bot.start();
		return 0;
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}