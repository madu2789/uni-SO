all: servidor

LinkedList.o: ../utils/LinkedList.c ../utils/LinkedList.h
	gcc -Wall -Wextra -c ../utils/LinkedList.c

facadeLL.o: facadeLL.c ../utils/facadeLL.h
	gcc -Wall -Wextra -c ../utils/facadeLL.c

socketServer.o: socketServer.c ../utils/socketServer.h
	gcc -Wall -Wextra -c ../utils/socketServer.c

log.o: log.c ../utils/log.h
	gcc -Wall -Wextra -c ../utils/log.c

sincro.o: sincro.c ../utils/sincro.h
	gcc -Wall -Wextra -c ../utils/sincro.c

servidor.o: servidor.c servidor.h
	gcc -Wall -Wextra -c servidor.c


servidor: servidor.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/socketServer.o ../utils/log.o ../utils/sincro.o
	gcc -Wall -Wextra servidor.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/socketServer.o ../utils/log.o ../utils/sincro.o -o servidor.exe