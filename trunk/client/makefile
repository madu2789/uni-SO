all: client

client.o: client.c client.h
	gcc -Wall -Wextra -c client.c
LinkedList.o: ../utils/LinkedList.c ../utils/LinkedList.h
	gcc -Wall -Wextra -c ../utils/LinkedList.c
	
client: client.o ../utils/LinkedList.o 
	gcc -Wall -Wextra client.o ../utils/LinkedList.o -o client.exe