NAME		= webserv

CXX			= c++
CXXFLAGS	= -std=c++98 -pedantic-errors -Wall -Wextra -Werror -MMD -MP
INCLUDE		= -I$(INC_DIR)

SRC_DIR		= ./srcs
OBJ_DIR		= ./obj
INC_DIR		= ./inc

SRCS		= $(shell cd $(SRC_DIR) && find * -name "*.cpp" -and ! -name "main*.cpp" -and ! -name "utest*.cpp")

### OS dependency
OS			= $(shell uname -s)
ifeq ($(OS), Darwin)
# TODO: check flags: -pedantic-errors -Wall -Wextra -Werror
CXXFLAGS	= -std=c++98 -MMD -MP
endif
###

ifeq ($(MAKECMDGOALS), mock)
	CXXFLAGS += -DMOCK
endif

ifeq ($(MAKECMDGOALS), request_parse_test)
	SRCS += ./test_main/main_request_parse.cpp
else ifeq ($(MAKECMDGOALS), unit_test)
	CXXFLAGS	= -std=c++11
	INCLUDE		+= -I$(gtestdir)
	SRCS		= $(shell cd $(SRC_DIR) && find * -name "*.cpp" -and ! -name "main*.cpp")
else
	SRCS += main.cpp
endif

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

OK_OR_KO		=	"KO"
TEST_SH			=	"./test/test.sh"
REQUEST_PARSE	=	"request_parse"

request_parse_test: $(OBJ_SUBDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDE) -owebserv
	$(TEST_SH) $(OK_OR_KO) $(REQUEST_PARSE)

gtestdir		=	./test
unit_testdir	=	$(SRC_DIR)/unit_test
gtest			=	$(gtestdir)/gtest $(gtestdir)/googletest-release-1.11.0

$(gtest):
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	$(RM) -rf release-1.11.0.tar.gz
	python3 googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(gtestdir)
	mv googletest-release-1.11.0 $(gtestdir)

unit_test: $(gtest) $(OBJ_SUBDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc -I$(gtestdir) $(INCLUDE) -lpthread -pthread -owebserv
	./webserv

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

.PHONY : mock
mock: fclean $(NAME)

-include $(DEPS)
