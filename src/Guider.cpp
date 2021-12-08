#include "Client.h"

#include <iostream>

#include <string>
#include <sstream>

const char* hosts[] = {"172.16.99.100", "172.16.99.101"};
Client connections[16] = {};

void connectToClients() {
  for (int i = 0; hosts[i] != 0; i++) {
    Client client(hosts[i], 8080);
    connections[i] = client;
  }
}

int main(int argc, char const *argv[])
{
  connectToClients();

  while (1) {
    char wemos1Buffer[1024] = {0};

    // TODO: Idea change to 2 threads 1 doing operation logic and one writing new values / reading new values from the server.

    // Read from Wemos 1
    connections[0].send("3:0\n");
    connections[0].receive(wemos1Buffer, 1024);

    // Debug send the buffer
    printf("Buffer1: %s", wemos1Buffer);

    // Send to Wemos 2
    std::ostringstream oss;
    oss << "1:" << wemos1Buffer[0] << "\n";
    std::string string = oss.str();

    char wemos2Buffer[1024] = {0};

    connections[1].send(string.c_str());
    connections[1].receive(wemos2Buffer, 1024);

    // Debug send the buffer
    printf("Buffer2: %s", wemos2Buffer);

    // Sleep for a bit.
    usleep(500000);
  }
}