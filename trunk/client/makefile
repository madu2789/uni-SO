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

transfer.o: ../utils/transfer.c ../utils/transfer.h
	gcc -Wall -Wextra -c ../utils/transfer.c

sincroClient.o: sincroClient.c ../utils/sincroClient.h
	gcc -Wall -Wextra -c ../utils/sincroClient.c

initInCommon.o: initInCommon.c ../utils/initInCommon.h
	gcc -Wall -Wextra -c ../utils/initInCommon.c

client.o: client.c client.h
	gcc -Wall -Wextra -c client.c


client: client.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/md5.o ../utils/socketClient.o ../utils/log.o ../utils/transfer.o ../utils/sincroClient.o ../utils/initInCommon.o
	gcc -Wall -Wextra client.o ../utils/LinkedList.o ../utils/facadeLL.o ../utils/md5.o ../utils/socketClient.o ../utils/log.o ../utils/transfer.o ../utils/sincroClient.o ../utils/initInCommon.o -o client.exe