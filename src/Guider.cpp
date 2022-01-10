#define DASHBOARD_PORT

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Client.h"
#include "Server.h"

#include "ModuleAddresses.h"

#include "Dashboard.h"
#include "Door.h"
#include "TableLamp.h"
#include "Chair.h"
#include "Bed.h"

#include "Timer.h"

// Declair dashboard server and class
Server server(8000);
Dashboard dashboardModule;

// Declair an instance of the module
Chair chair;

// Declair timers
Timer doorLightTimer = Timer(5);
Timer tableLampTimer = Timer(2);
Timer bedTimer = Timer(10);
Timer chairToggleTimer = Timer(1);

// Declair the functions used in seperate threads.
void fetcher();
void logic();
void dashboard();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  Client chairClient(CHAIR_MODULE, 8080);

  // Create a new module using the connection created above.

  chair = Chair(chairClient);

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
    chair.fetch();

    // Sleep for a bit because we only have 2 module and we don't want to overload them.
    usleep(100000);
    std::cout << "Fetching round...\n";
  }
}

int isNightTime(std::time_t current)
{
  return (localtime(&current)->tm_hour >= 19 && localtime(&current)->tm_hour <= 6);
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    // Chair logic
    while (chair.getLock())
      ;

    chair.lock();
    if (chair.getSwitch() && chair.getFsensor() > 100 && chairToggleTimer.finished())
    {
      chair.switchCurrent = !chair.switchCurrent;
      chairToggleTimer.start();
    std::cout << chair.switchCurrent << "\n";

    }

    if (chair.getFsensor() <= 100) {
      chair.switchCurrent = false;
    }

    chair.setLed(chair.switchCurrent);
    chair.setMotor(chair.switchCurrent);
    chair.unlock();
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

      while (dashboardModule.getLock())
        ;
      dashboardModule.lock();

      if (document.HasMember("openDoor") && document["openDoor"].IsBool())
      {
        dashboardModule.setDoor(document["openDoor"].GetBool());
      }

      if (document.HasMember("lampColor") && document["lampColor"].IsInt())
      {
        dashboardModule.setLampColor(document["lampColor"].GetInt());
      }

      if (document.HasMember("motionAlert") && document["motionAlert"].IsBool())
      {
        dashboardModule.setMotionAlert(document["motionAlert"].GetBool());
      }

      std::cout << "Recieved!  " << buffer << "\n";

      server.send(socket_fd, dashboardModule.getJSON().c_str());
      dashboardModule.unlock();
    }

    close(socket_fd);

    receiveStatus = 0;
  }
}