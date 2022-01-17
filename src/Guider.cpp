#define DASHBOARD_PORT

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>

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
#include "Column.h"

#include "Timer.h"

// Declare dashboard server and class
Server server(8000);
Dashboard dashboardModule;

using namespace std;
// Declare an instance of the module
Column column;
Chair chair;
Bed bed;
TableLamp tableLamp;
Door door;

// Declare timers
Timer doorLightTimer = Timer(5);
Timer tableLampTimer = Timer(2);
Timer bedTimer = Timer(10);
Timer chairToggleTimer = Timer(1);
Timer columnBuzzer = Timer(1);

// Declare the functions used in seperate threads.
void fetcher();
void logic();
void dashboard();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // dashboardModule = Dashboard();

  // Create a new connection to the Wemos board.
  std::cout << "Connecting to Column.."
            << "\n";
  Client columnClient(COLUMN_MODULE, 8080);
  /*
  std::cout << "Connecting to Chair.."
            << "\n";
  Client chairClient(CHAIR_MODULE, 8080);
  std::cout << "Connecting to Bed.."
            << "\n";
  Client bedClient(BED_MODULE, 8080);
  std::cout << "Connecting to Lamp.."
            << "\n";
  Client lampClient(LAMP_MODULE, 8080);
  std::cout << "Connecting to Door.."
            << "\n";
  Client doorClient(DOOR_MODULE, 8080);
  */

  // Create a new module using the connection created above.
  column = Column(columnClient);
  /*
  bed = Bed(bedClient);
  tableLamp = TableLamp(lampClient);
  door = Door(doorClient);
  chair = Chair(chairClient);
  */

  // Spin up the three threads.
  std::thread fetcherThread(fetcher);
  std::thread logicThread(logic);
  std::thread dashboardThread(dashboard);

  // Close down the threads when they are finished.
  fetcherThread.join();
  logicThread.join();
  dashboardThread.join();
}

// Updates the module objects syncing them with the wemos hardware.
void fetcher()
{
  while (1)
  {
    // Synchronize the object with the Wemos module
    std::cout << "Fetching Column...\n";
    column.fetch();
    /*
    std::cout << "Fetching Chair...\n";
    chair.fetch();
    std::cout << "Fetching TableLamp...\n";
    tableLamp.fetch();
    std::cout << "Fetching Door...\n";
    door.fetch();
    std::cout << "Fetching Bed...\n";
    bed.fetch();
*/
    std::cout << "Starting new fetching round...\n";
  }
}

int isNightTime(std::time_t current)
{
  return !(localtime(&current)->tm_hour >= 19 && localtime(&current)->tm_hour <= 6);
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    std::time_t current = std::time(nullptr);

    // Column logic
    while (column.getLock() || dashboardModule.getLock())
      ;
    column.lock();
    dashboardModule.lock();
    if (column.getButton())
    {
      cout << "DE ALARMKNOP IS INGEDRUKT! DE BEWONER IS IN NOOD!" << endl;
      column.setLed(true);
    }
    if (column.getBuzzer() > 50)
    {
      cout << "ER IS BRAND!" << endl;
      for (int i = 0; i < 3; i++)
      {
        column.setBuzzer(true);
        columnBuzzer.start();
        if (columnBuzzer.finished())
        {
          column.setBuzzer(false);
        }
      }
    }
    if (door.getButtonOut())
    {
      columnBuzzer.start();
      column.setBuzzer(true);
    }
    if (columnBuzzer.finished())
    {
      column.setBuzzer(false);
    }

    // Table Lamp Logic, turns white on motion, otherwise it runs to the dashboard provided color.
    while (tableLamp.getLock() || dashboardModule.getLock())
      ;
    tableLamp.lock();
    dashboardModule.lock();

    if (tableLamp.getPirSensor() && isNightTime(current))
    {
      tableLamp.setLed(255, 255, 255);
      tableLampTimer.start();
    }
    else
    {
      if (tableLampTimer.finished() || !isNightTime(current))
        tableLamp.setLed(dashboardModule.getLampColor());
    }

    tableLamp.unlock();
    dashboardModule.unlock();

    // Door Logic
    while (door.getLock() || dashboardModule.getLock())
      ;

    door.lock();
    dashboardModule.lock();

    if (door.getButtonIn())
    {
      door.setDoor(180).setLedIn(false).setLedOut(false);
      doorLightTimer.stop();
    }
    else if (door.getButtonOut())
    {
      if (isNightTime(current))
      {
        door.setLedIn(true).setLedOut(true);
        doorLightTimer.start();
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

    if (dashboardModule.getDoor())
      door.setDoor(180);
    door.unlock();
    dashboardModule.unlock();

    // Bed Logic
    while (bed.getLock())
      ;
    bed.lock();
    if ((bed.getps() >= 100) && isNightTime(current))
    {
      bed.switchPast = bed.switchCurrent;

      if (bed.getsw())
      {
        bed.switchCurrent = bed.getsw();
      }
      else if (!bed.getsw())
      {
        bed.switchCurrent = bed.getsw();
      }
      if (!bed.switchCurrent && bed.switchPast)
      {
        bed.setled(!bed.getLed());
        bedTimer.start();
      }
      if (bedTimer.finished())
      {
        bed.setled(0);
      }
    }
    else if (!bedTimer.finished())
    {
      bedTimer.start();
    }
    bed.unlock();

    while (bed.getLock() || tableLamp.getLock() || dashboardModule.getLock())
      ;
    // Bed / Night detection
    bed.lock();
    tableLamp.lock();
    dashboardModule.lock();

    if (bed.getps() > 100 && isNightTime(current) && tableLamp.getPirSensor())
    {
      dashboardModule.setMotionAlert(true);
    }

    bed.unlock();
    tableLamp.unlock();
    dashboardModule.unlock();

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

    if (chair.getFsensor() <= 100)
    {
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