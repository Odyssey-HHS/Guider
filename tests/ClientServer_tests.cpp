#include "../utilities/Server.cpp"
#include "../utilities/Client.cpp"

#include <iostream>
#include <thread>
#include <future>

char clientSending[] = "Hello World!";

void serverOperations(std::promise<char *> &&p)
{
  std::cout << "[TESTS] Creating a server\n";
  Server server;

  std::cout << "[TESTS] Listening for incoming connections\n";

  char serverIncoming[1024]{0};

  if (server.receive(serverIncoming, 1024) < 0)
  {
    perror("Failed to accept incoming connection.\n");
  }

  std::cout << "[TESTS] Received message! Existing\n";
  p.set_value(serverIncoming);
}

void clientOperations()
{
  std::cout << "[TESTS] Creating a client\n";

  Client client("127.0.0.1", 8000);

  std::cout << "[TESTS] Sending data!\n";

  client.send(clientSending, 13);
}

int main(int argc, char const *argv[])
{
  std::promise<char *> p;
  auto f = p.get_future();
  std::thread serverThread(serverOperations, std::move(p));

  usleep(1000);

  std::cout << "[TESTS] Waiting 1 second for server thread\n";
  sleep(1);

  std::cout << "[TESTS] Starting client thread\n";
  std::thread clientThread(clientOperations);

  serverThread.join();
  clientThread.join();
  std::cout << "[TESTS] Threads closed, compairing results...\n";

  char *test = f.get();

  std::cout << "Incoming: " << test << "\nOutgoing: " << clientSending << "\n";

  int i;
  for (i = 0; i < strlen(clientSending); i++)
  {
    if (clientSending[i] != test[i])
    {
      std::cout << "[ERROR] Incoming message wan't the same as outgoing!\n";
      break;
    }
  }

  for (; i < strlen(test); i++)
  {
    if (test[i] != 0)
    {
      std::cout << "[ERROR] Incoming message had garbage after the real message! : '" << (int)test[i] << "'\n";
      break;
    }
  }

  std::cout << "[TESTS] Finished!\n";
  return 0;
}
