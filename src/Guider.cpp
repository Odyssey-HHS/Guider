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

  if (server.receive(buffer, 1024) < 0)
  {
    perror("Failed to accept incoming connection.\n");
  }

  printf("%s\n", buffer);

  std::cout << "Received message! Existing\n";

  /* code */
  return 0;
}
