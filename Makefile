NAME		=	webserv

OBJ_DIR		=	./.obj/

SRC_DIR		=	./src/

SRCS		=   ${SRC_DIR}main.cpp		${SRC_DIR}utils.cpp\
				${SRC_DIR}WebServer.cpp	${SRC_DIR}ConfigFile.cpp\
				${SRC_DIR}Log.cpp		${SRC_DIR}Socket.cpp\
\
				${SRC_DIR}ADirective.cpp	${SRC_DIR}Http.cpp\
				${SRC_DIR}Include.cpp		${SRC_DIR}Server.cpp\
				${SRC_DIR}Listen.cpp		${SRC_DIR}Root.cpp\
				${SRC_DIR}ServerName.cpp

OBJS        =	$(addprefix $(OBJ_DIR),$(notdir $(SRCS:.cpp=.o)))

CXX			=	c++

RM			=	rm -rf

CXXFLAGS	=	-Wall -Wextra -Werror -Wconversion -g -std=c++98 -Iinclude/

ARGS		=	input.txt

MAKEFLAGS	+=	--silent


${OBJ_DIR}%.o:	${SRC_DIR}%.cpp
				@mkdir -p ${OBJ_DIR}
				${CXX} ${CXXFLAGS} -c $< -o $@

${NAME}:		${OBJS}
				@echo "Compilation of ${NAME} ..."
				${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}
				@echo "${NAME} created[0m ✔️"

all:			${NAME}

clean:
				@ echo "Deleting ${NAME} objs ✔️"
				@${RM} ${OBJ_DIR}


fclean:			clean
				@ echo "Deleting ${NAME} binary ✔️"
				@${RM} ${NAME}

re:				fclean all

run:			re
				./${NAME} ${ARGS}

.PHONY:			all clean fclean re bonus