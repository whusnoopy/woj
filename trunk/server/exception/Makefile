CC=$(CXX)
CXXFLAGS=-I../ \
				 -I/usr/include/libxml2 \
				 -L/usr/lib/libxml2 -I../ \
				 -I/usr/include/mysql -L/usr/lib/mysql \
				 -I../../ \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD

ALL_SOURCES:=$(filter-out $(wildcard test_*.cc),$(wildcard *.cc))

# All objects will auto build by "$(CXX) $(CXXFLAGS) -c" if there is no rule
# to make it
ALL_OBJS:=$(patsubst %.cc,%.o,$(ALL_SOURCES))

.PHONY: all clean rebuild test

all : $(ALL_OBJS)

clean :
	@rm -f $(ALL_OBJS) *.d

rebuild : clean all

