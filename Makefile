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
else ifeq ($(MAKECMDGOALS), request_parse_status_code_test)
	SRCS += ./test_main/main_request_parse_status_code.cpp
else ifeq ($(MAKECMDGOALS), unit_test)
	CXXFLAGS	= -std=c++11
	INCLUDE		+= -I$(gtestdir)
	SRCS		= $(shell cd $(SRC_DIR) && find * -name "*.cpp" -and ! -name "main*.cpp")
else ifeq ($(MAKECMDGOALS), unit_test_bonus)
	CXXFLAGS	= -std=c++11 -DBONUS
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

### REQUEST PARSE TEST by shellscript
OK_OR_KO				=	"KO"
REQUEST_PARSE_TEST_DIR	=	"./test/request_parse_test"
REQUEST_PARSE_TEST_SH	=	"./run.sh"

request_parse_test: $(NAME)
	cd $(REQUEST_PARSE_TEST_DIR) && $(REQUEST_PARSE_TEST_SH) $(OK_OR_KO)
###

### REQUEST PARSE STATUS CODE TEST by shellscript
OK_OR_KO				=	"KO"
REQUEST_PARSE_STATUS_CODE_TEST_DIR	=	"./test/request_parse_test"
REQUEST_PARSE_STATUS_CODE_TEST_SH	=	"./run_status_code_test.sh"

request_parse_status_code_test: $(NAME)
	cd $(REQUEST_PARSE_STATUS_CODE_TEST_DIR) && $(REQUEST_PARSE_STATUS_CODE_TEST_SH) $(OK_OR_KO)
###

### UNIT TEST by GoogleTest
gtestdir		=	./test/unit_test
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

unit_test_bonus: unit_test
###

### SYSTEM TEST by pytest
SYSTEM_TEST_DIR	=	"./test/system_test"
SYSTEM_TEST_SH	=	"./run.sh"

system_test: $(NAME)
	cd $(SYSTEM_TEST_DIR) && $(SYSTEM_TEST_SH)
###

### STRESS TEST by k6
.PHONY: stress_test
STRESS_TEST_DIR	=	"./test/stress_test"
STRESS_TEST_SH	=	"./run.sh"

K6_DIR			=	$(STRESS_TEST_DIR)/k6
K6_REPO			=	"https://github.com/grafana/k6"

GOLANG_DIR		=	$(STRESS_TEST_DIR)/go
GOLANG_BASE_URL	=	https://golang.org/dl/
GO_VERSION		=	1.22.3
GOLANG_GIP_FILE	=	go$(GO_VERSION).linux-arm64.tar.gz

ifeq ($(OS), Darwin)
	GOLANG_GIP_FILE	=	go$(GO_VERSION).darwin-amd64.tar.gz
endif

$(GOLANG_DIR):
	@if [ ! -d $(GOLANG_DIR) ]; then \
		curl -LO $(GOLANG_BASE_URL)$(GOLANG_GIP_FILE); \
		tar -xzf $(GOLANG_GIP_FILE); \
		mv go $(GOLANG_DIR); \
		rm $(GOLANG_GIP_FILE); \
	fi

$(K6_DIR):
	@if [ ! -d $(K6_DIR) ]; then \
		git clone $(K6_REPO) $(K6_DIR); \
		cd $(K6_DIR); \
		../go/bin/go build; \
	fi

stress_test: $(GOLANG_DIR) $(K6_DIR) $(NAME)
	@cd $(STRESS_TEST_DIR) && $(STRESS_TEST_SH)
###


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
