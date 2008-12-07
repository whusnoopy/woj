CC=$(CXX)
CXXFLAGS=-I/usr/include/mysql -L/usr/lib/mysql \
				 -I../../ \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD

ALL_SOURCES:=$(wildcard *.cc)

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
ALL_OBJS:=$(patsubst %.cc,%.o,$(ALL_SOURCES))

.PHONY: all clean rebuild test

all : $(ALL_OBJS)

clean :
	@rm -f $(ALL_OBJS) *.d

rebuild : clean all

