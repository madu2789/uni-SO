all: servidor client




LinkedList.o: utils/LinkedList.c utils/LinkedList.h
	gcc -Wall -Wextra -c utils/LinkedList.c

facadeLL.o: facadeLL.c utils/facadeLL.h
	gcc -Wall -Wextra -c utils/facadeLL.c

socketServer.o: socketServer.c utils/socketServer.h
	gcc -Wall -Wextra -c utils/socketServer.c

log.o: log.c utils/log.h
	gcc -Wall -Wextra -c utils/log.c

transfer.o: utils/transfer.c utils/transfer.h
	gcc -Wall -Wextra -c utils/transfer.c

sincro.o: sincroServer.c utils/sincroServer.h
	gcc -Wall -Wextra -c utils/sincroServer.c

initInCommon.o: initInCommon.c utils/initInCommon.h
	gcc -Wall -Wextra -c utils/initInCommon.c

servidor.o: servidor/servidor.c servidor/servidor.h
	gcc -Wall -Wextra -c servidor/servidor.c


servidor: servidor.o utils/LinkedList.o utils/facadeLL.o utils/socketServer.o utils/log.o utils/transfer.o utils/sincroServer.o utils/initInCommon.o
	gcc -pthread -Wall -Wextra servidor.o utils/LinkedList.o utils/facadeLL.o utils/socketServer.o utils/log.o utils/transfer.o utils/sincroServer.o utils/initInCommon.o -o servidor/servidor.exe




md5.o: md5.c utils/md5.h
	gcc -Wall -Wextra -c utils/md5.c

socketClient.o: socketClient.c utils/socketClient.h
	gcc -Wall -Wextra -c utils/socketClient.c

sincroClient.o: sincroClient.c utils/sincroClient.h
	gcc -Wall -Wextra -c utils/sincroClient.c

client.o: client/client.c client/client.h
	gcc -Wall -Wextra -c client/client.c


client: client.o utils/LinkedList.o utils/facadeLL.o utils/md5.o utils/socketClient.o utils/log.o utils/transfer.o utils/sincroClient.o utils/initInCommon.o
	gcc -pthread -Wall -Wextra client.o utils/LinkedList.o utils/facadeLL.o utils/md5.o utils/socketClient.o utils/log.o utils/transfer.o utils/sincroClient.o utils/initInCommon.o -o client/client.exe




clean:
	rm -vf client.o client/client.exe servidor.o servidor/servidor.exe utils/*.o