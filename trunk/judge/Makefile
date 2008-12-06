default : all

all : .client

clean :
	@make -C client clean

install : all
	@echo "Install judge_client to ../bin"
	@cp client/judge_client ../bin/judge_client
	@echo "Install judge scripts to ../bin"
	@cp bin/*.sh ../bin/

.client : 
	make -C client

