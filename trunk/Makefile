default : all

all : .base .judge .server

clean :
	@make -C base clean
	@make -C judge clean
	@make -C server clean
	@rm -rf bin

install : all
	@rm -rf bin
	@mkdir bin
	@make -C judge install
	@make -C server install
	@echo "Install Online Judge to /home/flood/worktemp/bin/"
	@cp -r bin /home/flood/worktemp/

.base :
	make -C base

.judge : 
	make -C judge

.server :
	make -C server

