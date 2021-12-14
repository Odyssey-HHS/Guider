#include <iostream>

//#include "ExampleModule.h"
#include "ModuleAddresses.h"
#include "Door.h"
#include <thread>

// Declair an instance of the module
//ExampleModule testModule;
Door door;

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  //Client client(EXAMPLE_MODULE, 8080);
  Client doorClient(DOOR_MODULE, 8080);

  // Create a new module using the connection created above.
  //testModule = ExampleModule(client);
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

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    // Example door logic, this is just an example and should be cleaned up for use with multiple modules.
    while (door.getLock());
    door.lock();
    if (door.getButtonIn())
    {
      door.setDoor(0);
    }
    else if (door.getButtonOut())
    {
      door.setDoor(180);
    }
    else
    {
      door.setDoor(65);
    }
    door.unlock();
  }
}