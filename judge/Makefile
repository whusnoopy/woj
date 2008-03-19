default : all

all : .kernel_module .client

clean :
	@make -C kernel_module clean
	@make -C client clean

install : all

.kernel_module :
	make -C kernel_module

.client : .kernel_module
	make -C client

