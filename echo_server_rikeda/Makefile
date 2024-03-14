TARGET			=	webserv
SERVER			=	server
PROXY			=	proxy

CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -Werror
CXXFLAGS		+=	-std=c++98
RM				=	rm -rf

PRE_OPT			=	-MMD -MP
INCLUDE			=	-I$(INCLUDES_DIR)

SRCS_DIR		=	./srcs
OBJS_DIR		=	./objs
INCLUDES_DIR	=	./inc

SRCS			=	Server.cpp \
					ConnectionManager.cpp \
					Connection.cpp

SERVER_SRCS		=	ServerHandler.cpp \
					server_main.cpp

PROXY_SRCS		=	ProxyHandler.cpp \
					proxy_main.cpp

OBJS			=	$(patsubst %.o, $(OBJS_DIR)/%.o, $(SRCS:.cpp=.o))
SERVER_OBJS		=	$(patsubst %.o, $(OBJS_DIR)/%.o, $(SERVER_SRCS:.cpp=.o))
PROXY_OBJS		=	$(patsubst %.o, $(OBJS_DIR)/%.o, $(PROXY_SRCS:.cpp=.o))
SERVER_OBJS		+=	$(patsubst %.o, $(OBJS_DIR)/%.o, $(SRCS:.cpp=.o))
PROXY_OBJS		+=	$(patsubst %.o, $(OBJS_DIR)/%.o, $(SRCS:.cpp=.o))

DEPS			=	$(OBJS:.o=.d)

# SERVER_SRCS		:=	$(addprefix $(SRCS_DIR)/, $(SERVER_SRCS))
# PROXY_SRCS		:=	$(addprefix $(SRCS_DIR)/, $(PROXY_SRCS))


$(OBJS_DIR)	: % :
	@mkdir -p $@

$(TARGET):	$(OBJS_DIR) $(SERVER) $(PROXY)

$(SERVER):	$(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SERVER_OBJS) -o $@

$(PROXY):	$(PROXY_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(PROXY_OBJS) -o $@

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(PRE_OPT) -o $@ -c $<

.DEFAULT_GOAL	=	all
.PHONY:	all
all:	$(TARGET)

.PHONY:	clean
clean:
	$(RM) $(OBJS_DIR)

.PHONY:	fclean
fclean:	clean
	$(RM) $(SERVER) $(PROXY)

.PHONY:	re
re:	fclean all

-include $(DEPS)
