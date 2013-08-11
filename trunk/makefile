all: servidor client




servidor.o: servidor/servidor.c servidor/servidor.h
	gcc -Wall -Wextra -c servidor/servidor.c

sincroServer.o: utils/sincroServer.c utils/sincroServer.h
	gcc -Wall -Wextra -c utils/sincroServer.c

initInCommon.o: utils/initInCommon.c utils/initInCommon.h
	gcc -Wall -Wextra -c utils/initInCommon.c

log.o: utils/log.c utils/log.h
	gcc -Wall -Wextra -c utils/log.c

facadeLL.o: utils/facadeLL.c utils/facadeLL.h
	gcc -Wall -Wextra -c utils/facadeLL.c

LinkedList.o: utils/LinkedList.c utils/LinkedList.h
	gcc -Wall -Wextra -c utils/LinkedList.c

socketServer.o: utils/socketServer.c utils/socketServer.h
	gcc -Wall -Wextra -c utils/socketServer.c

socketClient.o: utils/socketClient.c utils/socketClient.h
	gcc -Wall -Wextra -c utils/socketClient.c

transfer.o: utils/transfer.c utils/transfer.h
	gcc -Wall -Wextra .c utils/transfer.c

client.o: client/client.c client/client.h
	gcc -Wall -Wextra -c client/client.c

sincroClient.o: utils/sincroClient.c utils/sincroClient.h
	gcc -Wall -Wextra -c utils/sincroClient.c

md5.o: utils/md5.c utils/md5.h
	gcc -Wall -Wextra -c utils/md5.c




servidor: servidor/servidor.o utils/sincroServer.o utils/initInCommon.o utils/log.o utils/facadeLL.o utils/LinkedList.o utils/socketServer.o utils/socketClient.o utils/transfer.o
	gcc -pthread -Wall -Wextra servidor/servidor.o utils/sincroServer.o utils/initInCommon.o utils/log.o utils/facadeLL.o utils/LinkedList.o utils/socketServer.o utils/socketClient.o utils/transfer.o -o servidor/servidor.exe

client: client/client.o utils/socketClient.o utils/sincroClient.o utils/log.o utils/facadeLL.o utils/transfer.o utils/LinkedList.o utils/initInCommon.o utils/md5.o
	gcc -pthread -Wall -Wextra client/client.o utils/socketClient.o utils/sincroClient.o utils/log.o utils/facadeLL.o utils/transfer.o utils/LinkedList.o utils/initInCommon.o utils/md5.o -o client/client.exe




clean:
	rm -vf client/client.o client/client.exe servidor/servidor.o servidor/servidor.exe utils/*.o