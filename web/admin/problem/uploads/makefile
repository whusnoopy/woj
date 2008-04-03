objects = main.o result.o judge.o
L_PATH = -L/home/littleken/knuthocean/WOJ/mysql-standard-5.0.27-linux-x86_64-glibc23/lib
I_PATH = -I/home/littleken/knuthocean/WOJ/mysql-standard-5.0.27-linux-x86_64-glibc23/include
edit : main.o result.o judge.o
	g++ $(L_PATH) $(I_PATH)  -o  main main.o result.o judge.o -lmysqlclient  -lz
main.o: woj.h main.cpp
	g++ $(I_PATH)  -c main.cpp
result.o :woj.h
judge.o : woj.h 
.PHONY : clean
clean:
	-rm edit $(objects)
    
