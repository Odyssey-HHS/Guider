#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Server
{
  public:
    Server();
    Server(int port);
    int receive(char* buffer, int bufferLength);

  private:
    int listeningServer_fd = -1;
    struct sockaddr_in address;
    int createSocket();
    void attachSocket(int server_fd, int port);
    void listenSocket(int server_fd);
};
#endif