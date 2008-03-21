CC=$(CXX)
CXXFLAGS=-I../../ \
				 -I/usr/include/libxml2 \
				 -L/usr/lib/libxml2  \
	       -Wall -Werror -Wno-sign-compare -ansi -O3 -MD
VPATH=../util ../object ../../base
EXEFLAGS=-lxml2 \
				 -lpthread

SOURCES:=$(wildcard *.cc)
DEPS_SOURCES:=$(wildcard ../../base/*.cc)
UTIL_SOURCES:=$(wildcard ../util/*.cc)
OBJECT_SOURCES:=$(wildcard ../object/*.cc)

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
DEPS:=$(patsubst %.cc,%.o,$(DEPS_SOURCES))
UTIL:=$(patsubst %.cc,%.o,$(UTIL_SOURCES))
OBJECT:=$(patsubst %.cc,%.o,$(OBJECT_SOURCES))

.PHONY: all clean rebuild 

all: server

clean:
	@rm -f $(OBJS) *.d server

rebuild: clean all

server : $(OBJS) $(DEPS) $(UTIL) $(OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(EXEFLAGS)

