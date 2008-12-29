CC=$(CXX)
CXXFLAGS=-I./../ \
				 -I./ \
				 -I/usr/include/libxml2 -L/usr/lib/libxml2  \
				 -I/usr/include/mysql -L/usr/lib/mysql \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD -D_THIS_IS_SERVER_LOG_
EXEFLAGS=-lxml2 \
				 -lpthread \
				 -lmysqlclient

SOURCES:=$(wildcard *.cc)
DEPS_SOURCES:=$(filter-out $(wildcard ./../base/test_*.cc),$(wildcard ./../base/*.cc))
UTIL_SOURCES:=$(wildcard ./util/*.cc)
OBJECT_SOURCES:=$(wildcard ./object/*.cc)
DATA_SOURCES:=$(wildcard ./data/*.cc)
NETWORK_SOURCE:=$(wildcard ./network/*.cc)
JUDGECONTROL_SOURCES:=$(filter-out $(wildcard ./judgecontrol/test_*.cc),$(wildcard ./judgecontrol/*.cc))
EXCEPTION_SOURCES:=$(wildcard ./exception/*.cc)

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
DEPS:=$(patsubst %.cc,%.o,$(DEPS_SOURCES))
UTIL:=$(patsubst %.cc,%.o,$(UTIL_SOURCES))
OBJECT:=$(patsubst %.cc,%.o,$(OBJECT_SOURCES))
DATA:=$(patsubst %.cc,%.o,$(DATA_SOURCES))
NETWORK:=$(patsubst %.cc,%.o,$(NETWORK_SOURCE))
JUDGECONTROL:=$(patsubst %.cc,%.o,$(JUDGECONTROL_SOURCES))
EXCEPTION:=$(patsubst %.cc,%.o,$(EXCEPTION_SOURCES))

.PHONY: all clean rebuild 

all: oj_server

clean:
	@rm -f $(OBJS) *.d oj_server
	@make -C network clean
	@make -C data clean
	@make -C object clean
	@make -C util clean
	@make -C judgecontrol clean
	@make -C exception clean

rebuild: clean all

install: all
	@echo "Install oj_server to ../bin"
	@cp oj_server ../bin
	@echo "Install run oj_server scripts to ../bin"
	@cp start_*online_judge_server.sh ../bin
	@echo "Install configure.xml example to ../bin"
	@cp eg_configure.xml ../bin
	
oj_server: $(OBJS) $(DEPS) $(UTIL) $(OBJECT) $(DATA) $(JUDGECONTROL) $(NETWORK) $(EXCEPTION)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(EXEFLAGS)

