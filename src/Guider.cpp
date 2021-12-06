#include "Server.h"

#include <iostream>

/* Example server listening on port 8000. */
int main(int argc, char const *argv[])
{
  Server server;
  std::cout << "Server is running on default port\n";

  std::cout << "Listening for an message\n";

  // Create an empty buffer. (If this isn't empty stuff breaks! Default creation has random junk.)
  char buffer[1024] = {0};

  // Wait for an client
  int connection_fd = server.awaitClient();

  server.receive(connection_fd, buffer, 1024);
  server.send(connection_fd, buffer);

  server.send(connection_fd, "\n\nHello World!\n");

    std::cout << "Working fine!\n";
  return 0;
}
