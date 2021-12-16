#include <iostream>
#include <ctime>
#include <chrono>
#include "ModuleAddresses.h"
#include "Door.h"
#include <thread>

// Declair an instance of the module
// ExampleModule testModule;
Door door;

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  // Client client(EXAMPLE_MODULE, 8080);
  Client doorClient(DOOR_MODULE, 8080);

  // Create a new module using the connection created above.
  // testModule = ExampleModule(client);
  door = Door(doorClient);

  // Spin up the two threads.
  std::thread fetcherThread(fetcher);
  std::thread logicThread(logic);

  // Close down the threads when they are finished.
  fetcherThread.join();
  logicThread.join();
}

/* Updates the module objects syncing them with the wemos hardware. */
void fetcher()
{
  while (1)
  {
    // Sync module
    door.fetch();

    // Sleep for a bit because we only have one module and we don't want to overload it.
    usleep(100000);
  }
}

    std::time_t current;
    std::time_t doorLightTime;


/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    current = std::time(nullptr);
    struct tm *tm_struct = localtime(&current);

    // Example door logic, this is just an example and should be cleaned up for use with multiple modules.
    while (door.getLock())
      ;
    door.lock();
    if (door.getButtonIn())
    {
      door.setDoor(180).setLedIn(false).setLedOut(false);
      doorLightTime = std::time(nullptr);

    }
    else if (door.getButtonOut())
    {
      if (!(localtime(&current)->tm_hour >= 19 | localtime(&current)->tm_hour <= 6)) {
        door.setLedIn(true).setLedOut(true);
        doorLightTime = std::time(nullptr)+60;
      }
    } 
    else
    {
      door.setDoor(65);
    }
    if (current >= doorLightTime) {
      door.setLedIn(false).setLedOut(false);
    }
    door.unlock();
  }
}