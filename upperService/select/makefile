src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all : server

server:server.o upper.o 
	gcc server.o upper.o -o server -Wall 
%.o:%.c
	gcc -c $< -Wall

clean:
	rm $(obj) server
