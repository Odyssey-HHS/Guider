#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "ModuleAddresses.h"
#include "Door.h"
#include "TableLamp.h"
#include <thread>
#include "Timer.h"

// Declair an instance of the module
TableLamp tableLamp;
Door door;

Timer doorLightTimer = Timer(5);

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  //Client lampClient(LAMP_MODULE, 8080);
  std::cout<<"attempting connection"<<std::endl;
  Client doorClient(DOOR_MODULE, 8080);
  std::cout<<"connected"<<std::endl;
  // Create a new module using the connection created above.
  //tableLamp = TableLamp(lampClient);
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
    // Synchronize the object with the Wemos module
    tableLamp.fetch();
    // Sync module
    door.fetch();

    // Sleep for a bit because we only have one module and we don't want to overload it.
    usleep(100000);
  }
}

    std::time_t current;

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {

    // Example door logic, this is just an example and should be cleaned up for use with multiple modules.
    while (tableLamp.getLock())
      ;

    // Just as an example
    std::cout << "PID: " << tableLamp.getPirSensor() << "\n";

    // An simple example, ofcourse sleep can't be used.
    tableLamp.lock();
    tableLamp.setLed(255, 255, 255);
    tableLamp.unlock();
    //sleep(2);
    tableLamp.lock();
    tableLamp.setLed(255, 0, 0);
    tableLamp.unlock();
    //sleep(2);
    tableLamp.lock();
    tableLamp.setLed(0, 255, 0);
    tableLamp.unlock();
    //sleep(2);
    tableLamp.lock();
    tableLamp.setLed(0, 0, 255);
    tableLamp.unlock();
    //sleep(2);
    tableLamp.lock();
    tableLamp.setLed(0, 0, 0);
    tableLamp.unlock();
    //sleep(2);
    while (door.getLock())
      ;
    door.lock();
    if (door.getButtonIn())
    {
      door.setDoor(180).setLedIn(false).setLedOut(false);
      doorLightTimer.stop();

    }
    else if (door.getButtonOut())
    {
      if (!(localtime(&current)->tm_hour >= 19 | localtime(&current)->tm_hour <= 6)) {
        door.setLedIn(true).setLedOut(true);
        doorLightTimer.start();
      }
    } 
    else
    {
      door.setDoor(65);
    }
    if (doorLightTimer.finished()) {
      door.setLedIn(false).setLedOut(false);
    }
    door.unlock();
  }
}