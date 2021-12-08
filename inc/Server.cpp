#include "Server.h"

Server::Server()
{
  int server_fd = createSocket();
  attachSocket(server_fd, 8000);
  listenSocket(server_fd);
}

int Server::createSocket()
{
  int server_fd; // Server socket file descriptor

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    printf("Failed to create server socket.\n");
    exit(EXIT_FAILURE);
  }

  return server_fd;
}

void Server::attachSocket(int server_fd, int port)
{
  int opt = 1;

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    printf("Failed to bind socket on protocol level.\n");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    printf("Failed to bind socket on address level.\n");
    exit(EXIT_FAILURE);
  }
}

void Server::listenSocket(int server_fd)
{
  if (listen(server_fd, 3) < 0)
  {
    printf("Failed to listen to bound socket.\n");
    exit(EXIT_FAILURE);
  }

  listeningServer_fd = server_fd;
}

int Server::send(const int socket_fd, const char *msg) const
{
  if (socket_fd == -1)
  {
    perror("Failed to send message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return ::send(socket_fd, msg, strlen(msg), 0);
}

int Server::send(const int socket_fd, const char *msg, const int msgLength) const
{
  if (socket_fd == -1)
  {
    perror("Failed to send message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return ::send(socket_fd, msg, msgLength, 0);
}

int Server::receive(const int socket_fd, char *buffer, int bufferLength) const
{
  if (socket_fd == -1)
  {
    perror("Failed to receive message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return read(socket_fd, buffer, bufferLength);
}

int Server::awaitClient() const
{
  int newSocket_fd;

  int addrlen = sizeof(address);

  if ((newSocket_fd = accept(listeningServer_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return newSocket_fd;
}