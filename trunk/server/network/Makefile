CC=$(CXX)
CXXFLAGS=-I../../ \
				 -I../ \
				 -I/usr/include/libxml2 \
				 -L/usr/lib/libxml2  \
				 -I/usr/include/mysql \
				 -L/usr/lib/mysql \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD
VPATH=../util ../object ../../base
EXEFLAGS=-lxml2 \
				 -lpthread \
				 -lmysqlclient

SOURCES:=$(wildcard *.cc)
DEPS_SOURCES:=$(filter-out $(wildcard ../../base/test_*.cc),$(wildcard ../../base/*.cc))
UTIL_SOURCES:=$(wildcard ../util/*.cc)
OBJECT_SOURCES:=$(wildcard ../object/*.cc)
DATA_SOURCES:=$(wildcard ../data/*.cc)
JUDGECONTROL_SOURCES:=$(filter-out $(wildcard ../judgecontrol/test_*.cc),$(wildcard ../judgecontrol/*.cc))

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
DEPS:=$(patsubst %.cc,%.o,$(DEPS_SOURCES))
UTIL:=$(patsubst %.cc,%.o,$(UTIL_SOURCES))
OBJECT:=$(patsubst %.cc,%.o,$(OBJECT_SOURCES))
DATA:=$(patsubst %.cc,%.o,$(DATA_SOURCES))
JUDGECONTROL:=$(patsubst %.cc,%.o,$(JUDGECONTROL_SOURCES))

.PHONY: all clean rebuild 

all: server

clean:
	@rm -f $(OBJS) *.d server

rebuild: clean all

server : $(OBJS) $(DEPS) $(UTIL) $(OBJECT) $(DATA) $(JUDGECONTROL)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(EXEFLAGS)

