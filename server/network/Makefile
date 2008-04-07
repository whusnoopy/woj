CC=$(CXX)
CXXFLAGS=-I../../ \
				 -I../ \
				 -I/usr/include/libxml2 \
				 -L/usr/lib/libxml2  \
				 -I/usr/include/mysql \
				 -L/usr/lib/mysql \
				 -Wall -Werror -Wno-sign-compare -ansi -O3 -MD

SOURCES:=$(wildcard *.cc)

OBJS:=$(patsubst %.cc,%.o,$(SOURCES))

.PHONY: all clean rebuild 

all: $(OBJS)

clean:
	@rm -f $(OBJS) *.d 

rebuild: clean all

