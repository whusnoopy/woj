default : all

all : .base .judge .server

clean :
	@make -C base clean
	@make -C judge clean
	@make -C server clean

install : all

.base :
	make -C base

.judge : 
	make -C judge

.server :
	make -C server

