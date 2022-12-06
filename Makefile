
NAME			:= ircserv

CXXFLAGS		:= -Wall -Werror -Wextra -g3 -std=c++98

#DEBUG			:= -fsanitize=address

OPTI			:= -O3

INC				:=	Client.hpp \
					Server.hpp \
					CommandHandler.hpp \
					Command.hpp \
					Channel.hpp \
					utils.hpp

INC_DIR			:= inc/

INC_FILES		:= $(addprefix $(INC_DIR), $(INC))

SRCS			:=	main.cpp \
					Client.cpp \
					Server.cpp \
					CommandHandler.cpp \
					Command.cpp \
					Commands/UserCommand.cpp \
					Commands/JoinCommand.cpp \
					Commands/NickCommand.cpp \
					Commands/QuitCommand.cpp \
					Commands/PingCommand.cpp \
					Commands/PongCommand.cpp \
					Commands/PassCommand.cpp \
					Commands/PartCommand.cpp \
					Commands/ModeCommand.cpp \
					Commands/TopicCommand.cpp \
					Commands/PrivMsgCommand.cpp \
					Commands/NoticeCommand.cpp \
					Commands/KickCommand.cpp \
					Commands/InviteCommand.cpp \
					Commands/NamesCommand.cpp \
					Commands/ListCommand.cpp \
					Commands/CapCommand.cpp \
					Channel.cpp \
					utils.cpp

SRCS_DIR		:= srcs/

SRCS_FILES		:= $(addprefix $(SRCS_DIR), $(SRCS))

OBJS			:= $(SRCS:.cpp=.o)

OBJS_DIR		:= objs/

OBJS_FILES		:= $(addprefix $(OBJS_DIR), $(OBJS))

all:			$(NAME)

$(NAME):		$(OBJS_FILES)
				c++ $(CXXFLAGS) $(DEBUG) $(SRCS_FILES) -o $(NAME)

$(OBJS_DIR)%.o:	$(SRCS_DIR)%.cpp $(INC_FILES) Makefile
				mkdir -p $(OBJS_DIR)
				mkdir -p $(OBJS_DIR)/Commands
				c++ $(CXXFLAGS) $(DEBUG) -c $< -o $@ -I$(INC_DIR)

clean:
				rm -rf $(OBJS_DIR)

fclean:			clean
				rm -f $(NAME)

re:				fclean all

.PHONY:			all clean fclean re 



