#include "Client.h"
#include <iostream>

Client::Client() {
  connectedSocket_fd = -1;
}

Client::Client(const char *address)
{
  int socket_fd = createSocket(address, 8000);
  connect(socket_fd);
}

Client::Client(const char *address, const int port)
{
  int socket_fd = createSocket(address, port);
  connect(socket_fd);
}

int Client::createSocket(const char *address, const int port)
{
  int socket_fd;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Failed to create socket connection.\n");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
  {
    perror("Invalid address / Address not supported.\n");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

void Client::connect(int socket_fd)
{
  std::cout<<"in connect function"<<std::endl;
  if (::connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("Connection Failed.\n");
    exit(EXIT_FAILURE);
  }
  std::cout<<"connected"<<std::endl;

  connectedSocket_fd = socket_fd;
}

int Client::send(const char *msg) const
{
  if (connectedSocket_fd == -1)
  {
    perror("Failed to send message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return ::send(connectedSocket_fd, msg, strlen(msg), 0);
}

int Client::send(const char *msg, const int msgLength) const
{
  if (connectedSocket_fd == -1)
  {
    perror("Failed to send message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return ::send(connectedSocket_fd, msg, msgLength, 0);
}

int Client::receive(char *buffer, int bufferLength) const
{
  if (connectedSocket_fd == -1)
  {
    perror("Failed to receive message, socket isn't connected!\n");
    exit(EXIT_FAILURE);
  }

  return read(connectedSocket_fd, buffer, bufferLength);
}