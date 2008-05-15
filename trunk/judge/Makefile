default : all

all : .client

clean :
	@make -C client clean

install : all
	@echo "Install judge_client to ./bin"
	@cp client/judge_client bin/judge_client

.client : 
	make -C client

