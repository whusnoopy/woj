CC=$(CXX)
CXXFLAGS=-I./../ \
				 -I./ \
				 -I./../server/ \
				 -I/usr/include/libxml2 \
				 -L/usr/lib/libxml2  \
				 -I/usr/include/mysql \
				 -L/usr/lib/mysql \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD -D_THIS_IS_SERVER_LOG_
EXEFLAGS=-lxml2 \
				 -lpthread \
				 -lmysqlclient

SOURCES:=$(wildcard databasetranslator.cc)
MD5_SOURCES:=$(wildcard md5.cc)
REJUDGE_SOURCES:=$(wildcard rejudge_all.cc)
MOSTDILIGENPLAYER_SOURCES:=$(wildcard mostdiligenplayer.cc)
DEPS_SOURCES:=$(filter-out $(wildcard ./../base/test_*.cc),$(wildcard ./../base/*.cc))
OBJECT_SOURCES:=$(wildcard ./../server/object/user.cc ./../server/object/inc.cc)
DATA_SOURCES:=$(wildcard ./../server/data/*connection.cc ./../server/data/result.cc)

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
MD5:=$(patsubst %.cc,%.o,$(MD5_SOURCES))
REJUDGE:=$(patsubst %.cc,%.o,$(REJUDGE_SOURCES))
MDP_OBJS:=$(patsubst %.cc,%.o,$(MOSTDILIGENPLAYER_SOURCES))
DEPS:=$(patsubst %.cc,%.o,$(DEPS_SOURCES))
OBJECT:=$(patsubst %.cc,%.o,$(OBJECT_SOURCES))
DATA:=$(patsubst %.cc,%.o,$(DATA_SOURCES))

.PHONY: all clean rebuild 

all: main

clean:
	@rm -f $(OBJS) $(REJUDGE) $(MD5) $(MDP_OBJS) *.d main rejudge

rebuild: clean all

main: $(OBJS) $(DEPS) $(OBJECT) $(INC) $(DATA) $(RESULT) $(MD5)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(EXEFLAGS)

rejudge: $(REJUDGE) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

mdp: $(MDP_OBJS) $(DEPS) $(DATA) $(OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(EXEFLAGS)

