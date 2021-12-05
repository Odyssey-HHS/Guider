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
  Client(const char *address);
  Client(const char *address, const int port);
  int send(const char *msg) const;
  int send(const char *msg, const int length) const;
  int receive(char *buffer, int bufferLength) const;

private:
  struct sockaddr_in serv_addr;
  int connectedSocket_fd = -1;
  int createSocket(const char *address, const int port);
  void connect(int socket_fd);
};
#endif