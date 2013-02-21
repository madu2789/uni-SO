all: servidor

LinkedList.o: ../utils/LinkedList.c ../utils/LinkedList.h
	gcc -Wall -Wextra -c ../utils/LinkedList.c
facadeLL.o: facadeLL.c ../utils/facadeLL.h
	gcc -Wall -Wextra -c ../utils/facadeLL.c
servidor.o: servidor.c servidor.h
	gcc -Wall -Wextra -c servidor.c

servidor: servidor.o ../utils/LinkedList.o ../utils/facadeLL.o
	gcc -Wall -Wextra servidor.o ../utils/LinkedList.o ../utils/facadeLL.o -o servidor.exe