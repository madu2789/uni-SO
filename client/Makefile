/* **********************************************
*
* Client Makefile
*
* ********************************************** */
CC = gcc
CFLAGS = -Wall -Wextra

CLIENT_FILES = client.c
//CLIENT_HEADERS = client.h

LINKEDLIST_FILES = ../utils/LinkedList.c
//LINKEDLIST_HEADERS = ../utils/LinkedList.h

//MD5_FILES = ../utils/md5.c
//MD5_HEADERS = ../utils/md5.h


CLIENT_O: client.o
LINKEDLIST_O: ../utils/LinkedList.o
//MD5_O: ../utils/md5.o


CLIENT_PROGNAME: client.exe


all: client

client.o: $(CLIENT_FILES) //$(CLIENT_HEADERS)
	$(CC) $(CFLAGS) -c $(CLIENT_FILES)
LinkedList.o: $(LINKEDLIST_FILES) //$(LINKEDLIST_HEADERS)
	$(CC) $(CFLAGS) -c $(LINKEDLIST_FILES)
//md5.o: $(MD5_FILES) $(MD5_HEADERS)
//	$(CC) $(CFLAGS) -c $(MD5_FILES)
	
client: $(CLIENT_O) $(LINKEDLIST_O) //$(MD5_O)
	$(CC) $(CFLAGS) $(CLIENT_O) $(LINKEDLIST_O) /*$(MD5_O)*/ -o $(CLIENT_PROGNAME)