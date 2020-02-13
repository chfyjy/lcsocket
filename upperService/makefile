all : client server

client:client.o
	gcc client.o -o client
client.o:client.c upper.h
	gcc -c client.c -include upper.h

server:server.o
	gcc server.o -o server

server.o:server.c upper.h
	gcc -c server.c -include upper.h


clean:
	rm *.o client server
