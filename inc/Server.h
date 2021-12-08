#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

class Server
{
public:
  Server();
  Server(int port);

  /* Send a message to the open socket connection.  Returns the number sent or -1. */
  int send(const int socket_fd, const char *msg) const;
  /* Send a message to the open socket connection.  Returns the number sent or -1. */
  int send(const int socket_fd, const char *msg, const int length) const;
  /* Receive a message from the open socket connection. Return the number read, -1 for errors or 0 for EOF. */
  int receive(const int socket_fd, char *buffer, int bufferLength) const;
  /* Await a client connection, this method is blocking. Returns the new socket fd */
  int awaitClient() const;

private:
  int listeningServer_fd = -1;
  struct sockaddr_in address;
  int createSocket();
  void attachSocket(int server_fd, int port);
  void listenSocket(int server_fd);
};
#endif