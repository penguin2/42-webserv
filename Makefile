NAME		= webserv

CXX			= c++
CXXFLAGS	= -std=c++98 -pedantic-errors -Wall -Wextra -Werror -MMD -MP
INCLUDE		= -I$(INC_DIR)

SRC_DIR		= ./srcs
OBJ_DIR		= ./obj
INC_DIR		= ./inc

SRCS		= $(shell cd $(SRC_DIR) && find * -name "*.cpp")
OBJS		= $(patsubst %.o, $(OBJ_DIR)/%.o, $(SRCS:.cpp=.o))
DEPS		= $(OBJS:.o=.d)
OBJ_SUBDIRS	= $(sort $(dir $(OBJS)))

RM			= rm -rf


$(OBJ_SUBDIRS) : % :
	@mkdir -p $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(NAME) : $(OBJ_SUBDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDE) -o $@


.DEFAULT_GOAL = all
.PHONY : all
all : $(NAME)

.PHONY : clean
clean :
	$(RM) $(OBJ_DIR)

.PHONY : fclean
fclean : clean
	$(RM) $(NAME)

.PHONY : re
re : fclean all


-include $(DEPS)
