all: client

LinkedList.o: ../utils/LinkedList.c ../utils/LinkedList.h
	gcc -Wall -Wextra -c ../utils/LinkedList.c
facadeLL.o: facadeLL.c ../utils/facadeLL.h
	gcc -Wall -Wextra -c ../utils/facadeLL.c
client.o: client.c client.h
	gcc -Wall -Wextra -c client.c

client: client.o ../utils/LinkedList.o ../utils/facadeLL.o
	gcc -Wall -Wextra client.o ../utils/LinkedList.o ../utils/facadeLL.o -o client.exe