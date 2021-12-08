#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

class Client
{
public:
  Client();
  Client(const char *address);
  Client(const char *address, const int port);

  /* Send a message to the open socket connection.  Returns the number sent or -1. */
  int send(const char *msg) const;
  /* Send a message to the open socket connection.  Returns the number sent or -1. */
  int send(const char *msg, const int length) const;
  /* Receive a message from the open socket connection. Return the number read, -1 for errors or 0 for EOF. */
  int receive(char *buffer, int bufferLength) const;

private:
  struct sockaddr_in serv_addr;
  int connectedSocket_fd = -1;
  int createSocket(const char *address, const int port);
  void connect(int socket_fd);
};
#endif