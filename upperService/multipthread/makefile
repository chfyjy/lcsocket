src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all : client server

client:client.o upper.o
	gcc client.o upper.o -o client -Wall
server:server.o upper.o 
	gcc server.o upper.o -o server -Wall -lpthread
%.o:%.c
	gcc -c $< -Wall

clean:
	rm $(obj) client server
