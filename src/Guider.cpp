#define DASHBOARD_PORT

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Server.h"
#include "ModuleAddresses.h"
#include "Door.h"
#include "TableLamp.h"
#include "Timer.h"

// Declair dashboard server
Server server(8000);

// Declair an instance of the module
TableLamp tableLamp;
Door door;

// Declair timers
Timer doorLightTimer = Timer(5);
Timer tableLampTimer = Timer(2);

// Declair the functions used in seperate threads.
void fetcher();
void logic();
void dashboard();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  Client lampClient(LAMP_MODULE, 8080);
  Client doorClient(DOOR_MODULE, 8080);

  // Create a new module using the connection created above.
  tableLamp = TableLamp(lampClient);
  door = Door(doorClient);

  // Spin up the two threads.
  std::thread fetcherThread(fetcher);
  std::thread logicThread(logic);
  std::thread dashboardThread(dashboard);

  // Close down the threads when they are finished.
  fetcherThread.join();
  logicThread.join();
  dashboardThread.join();
}

/* Updates the module objects syncing them with the wemos hardware. */
void fetcher()
{
  while (1)
  {
    // Synchronize the object with the Wemos module
    tableLamp.fetch();
    door.fetch();

    // Sleep for a bit because we only have 2 module and we don't want to overload them.
    usleep(100000);
    std::cout << "Fetching round...\n";
  }
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    std::time_t current = std::time(nullptr);

    // Example door logic, this is just an example and should be cleaned up for use with multiple modules.
    while (tableLamp.getLock())
      ;
    tableLamp.lock();
    if (tableLamp.getPirSensor())
    {
      tableLamp.setLed(255, 255, 255);
      tableLampTimer.start();
    }

    if (tableLampTimer.finished())
    {
      tableLamp.setLed(0, 0, 0);
    }
    tableLamp.unlock();

    // (Demo door logic)
    while (door.getLock() && tableLamp.getLock())
      ;

    door.lock();
    tableLamp.lock();
    if (door.getButtonIn())
    {
      door.setDoor(180).setLedIn(false).setLedOut(false);
      doorLightTimer.stop();
    }
    else if (door.getButtonOut())
    {
      if (!(localtime(&current)->tm_hour >= 19 | localtime(&current)->tm_hour <= 6))
      {
        door.setLedIn(true).setLedOut(true);
        doorLightTimer.start();
        tableLamp.setLed(255, 0, 0);
        tableLampTimer.start();
      }
    }
    else
    {
      door.setDoor(65);
    }

    if (doorLightTimer.finished())
    {
      door.setLedIn(false).setLedOut(false);
    }
    door.unlock();
    tableLamp.unlock();
  }
}

void dashboard()
{
  while (1)
  {
    std::cout << "Waiting for connection\n";
    // Wait for client (blocking)
    int socket_fd = server.awaitClient();

    std::cout << "Connected!\n";

    int receiveStatus = 0;

    // Disconnect after message failure.
    while (receiveStatus >= 0)
    {
      std::cout << "Waiting for message!\n";
      // Wait for message (blocking)
      char buffer[4096] = {0};
      receiveStatus = server.receive(socket_fd, buffer, 4096);

      // Parse a JSON string into DOM.
      rapidjson::Document document;
      document.Parse(buffer);

      if (!document.IsObject())
      {
        std::cout << "Didn't recieve JSON! Closing connection\n";
        receiveStatus = -1;
        break;
      }

      if (!document["module"].IsString())
      {
        std::cout << "JSON object didn't contain module string! Closing connection\n";
        receiveStatus = -1;
        break;
      }

      std::string moduleHeader = document["module"].GetString();
      Module *module = nullptr;

      if (moduleHeader.compare("light") == 0)
      {
        std::cout << "Light\n";
        module = nullptr;
      }

      if (moduleHeader.compare("door") == 0)
      {
        std::cout << "Door\n";
        module = nullptr;
      }

      while (module->getLock())
        ;

      module->lock();

      // TODO: @Wouter @Casper The setInputsJSON function needs to check if a value is given and then read it. The dashboard can't write the whole object because it might be old data and that would cause problems.

      module->setInputsJSON(buffer); // Set the inputs to dashboard values
      //module->setOutputsJSON(buffer); // Set the outputs to dashboard values

      module->unlock();

      std::cout << "Recieved!  " << buffer << "\n";
    }

    close(socket_fd);

    receiveStatus = 0;
  }
}