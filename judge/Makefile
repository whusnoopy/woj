default : all

all : .client

clean :
	@make -C client clean

install : all
	@echo "Install judge_client to ./bin"
	@cp client/judge_client bin/judge_client
	@cp -r bin /home/flood/worktemp/

.client : 
	make -C client

