NAME	= webserv

CC		= c++
CFLAGS	= -Wall -Wextra -Werror
STD		= -std=c++98

SRC_DIR	= src
OBJ_DIR	= .build
INC_DIR	= include

SRC		= main.cpp Server.cpp Error.cpp

SRCS	= $(SRC:%=$(SRC_DIR)/%)

OBJS	= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

INCS	= $(INC_DIR:%=-I%)

# Colors
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
WHITE	= \033[0;37m
BOLD	= \033[1m
UNSET	= \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCS) $(STD) $(SRCS) -o $(NAME)
	@echo "$(GREEN)[✓] $(WHITE)$(BOLD)$(NAME) created!$(UNSET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p .build
	$(CC) $(CFLAGS) $(STD) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)[!] $(WHITE)$(BOLD)$(NAME) object files deleted!$(UNSET)"

fclean: clean
	rm -f $(NAME)
	@echo "$(RED)[✕] $(WHITE)$(BOLD)$(NAME) deleted!$(UNSET)"

re: fclean all

.PHONY:
.SILENT:
