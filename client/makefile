all: client

LinkedList.o: ../utils/LinkedList.c ../utils/LinkedList.h
	gcc -Wall -Wextra -c ../utils/LinkedList.c

facadeLL.o: facadeLL.c ../utils/facadeLL.h
	gcc -Wall -Wextra -c ../utils/facadeLL.c

md5.o: md5.c ../utils/md5.h
	gcc -Wall -Wextra -c ../utils/md5.c

socketClient.o: socketClient.c ../utils/socketClient.h
	gcc -Wall -Wextra -c ../utils/socketClient.c

log.o: log.c ../utils/log.h
	gcc -Wall -Wextra -c ../utils/log.c

sincro.o: sincro.c ../utils/sincro.h
	gcc -Wall -Wextra -c ../utils/sincro.c

client.o: client.c client.h
	gcc -Wall -Wextra -c client.c


client: client.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/md5.o ../utils/socketClient.o ../utils/log.o ../utils/sincro.o
	gcc -Wall -Wextra client.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/md5.o ../utils/socketClient.o ../utils/log.o ../utils/sincro.o -o client.exe