CC=$(CXX)
CXXFLAGS=-I../../ \
	       -Wall -Werror -Wno-sign-compare -ansi -O3 -MD
EXEFLAGS=-I../kernel_module -L../kernel_module -lkmmon
DEPS=../../base/*.o

ALL_SOURCES:=$(wildcard *.cc)
TEST_SOURCES:=$(wildcard test_*.cc)
SOURCES=$(filter-out $(TEST_SOURCES),$(ALL_SOURCES))
ALL_OBJS:=$(patsubst %.cc,%.o,$(ALL_SOURCES))
OBJS:=$(patsubst %.cc,%.o,$(SOURCES))
TESTS:=$(patsubst %.cc,%,$(TEST_SOURCES))

.PHONY: all clean rebuild test

all: judge_client

clean:
	@rm -f $(ALL_OBJS) judge_client $(TESTS) *.d

rebuild: clean all

test: $(TESTS)

judge_client: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXEFLAGS) -o $@ $^ $(DEPS)

test_compile: test_compile.cc compile.o trace.o util.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPS) $(EXEFLAGS) 

test_judge: test_judge.cc judge.o trace.o util.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPS) $(EXEFLAGS) 

compile.o : compile.cc compile.h
	$(CXX) $(CXXFLAGS) -c $<

judge.o : judge.cc judge.h
	$(CXX) $(CXXFLAGS) -c $<

trace.o : trace.cc trace.h
	$(CXX) $(CXXFLAGS) -c $<

util.o : util.cc util.h
	$(CXX) $(CXXFLAGS) -c $<

