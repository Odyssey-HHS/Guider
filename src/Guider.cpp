#include "enableModules.h"

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Apartment.h"

Apartment apartment;

// Declare the functions used in seperate threads.
void fetcher();
void logic();
void dashboard();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Spin up the threads.
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
#ifdef USE_COLUMN
    std::cout << "Fetching apartment.getColumn()->..\n";
    apartment.getColumn()->fetch();
#endif
#ifdef USE_CHAIR
    std::cout << "Fetching Chair.\n";
    apartment.getChair()->fetch();
#endif
#ifdef USE_TABLELAMP
    std::cout << "Fetching TableLamp.\n";
    apartment.getTableLamp()->fetch();
#endif
#ifdef USE_DOOR
    std::cout << "Fetching Door.\n";
    apartment.getDoor()->fetch();
#endif
#ifdef USE_BED
    std::cout << "Fetching Bed.\n";
    apartment.getBed()->fetch();
#endif
#ifdef USE_WALL
    std::cout << "Fetching Wall.\n";
    apartment.getWall()->fetch();
#endif

    std::cout << "Starting new fetching round...\n";
  }
}

int isNightTime(std::time_t current)
{
  return (localtime(&current)->tm_hour >= 19 && localtime(&current)->tm_hour <= 6) | apartment.getDashboard()->getForceNightTime();
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    std::time_t current = std::time(nullptr);

    // Table Lamp Logic, turns white on motion, otherwise it runs to the dashboard provided color.
    while (apartment.getTableLamp()->getLock() || apartment.getDashboard()->getLock())
      ;
    apartment.getTableLamp()->lock();
    apartment.getDashboard()->lock();

    // Motion detection
    if (apartment.getTableLamp()->getPirSensor() && isNightTime(current))
    {
      apartment.getTableLamp()->setLed(255, 255, 255);
      apartment.getTableLamp()->timer.start();
    }
    else
    {
      // Dashboard default value
      if (apartment.getTableLamp()->timer.finished() || !isNightTime(current))
        apartment.getTableLamp()->setLed(apartment.getDashboard()->getLampColor());
    }

    apartment.getTableLamp()->unlock();
    apartment.getDashboard()->unlock();

    // Door Logic
    while (apartment.getDoor()->getLock() || apartment.getDashboard()->getLock())
      ;

    apartment.getDoor()->lock();
    apartment.getDashboard()->lock();

    // Inside button, opens door
    if (apartment.getDoor()->getButtonIn())
    {
      apartment.getDoor()->setDoor(180).setLedIn(false).setLedOut(false);
      apartment.getDoor()->timer.stop();
    }
    else if (apartment.getDoor()->getButtonOut())
    {
      // Outside button enables light on nighttime
      if (isNightTime(current))
      {
        apartment.getDoor()->setLedIn(true).setLedOut(true);
        apartment.getDoor()->timer.start();
      }
    }
    else
    {
      // Default closed value
      apartment.getDoor()->setDoor(65);
    }

    // Disable lights when timer is finished.
    if (apartment.getDoor()->timer.finished())
    {
      apartment.getDoor()->setLedIn(false).setLedOut(false);
    }

    // Overwrite door open value when dashboard requests this.
    if (apartment.getDashboard()->getDoor())
      apartment.getDoor()->setDoor(180);
    apartment.getDoor()->unlock();
    apartment.getDashboard()->unlock();

    // Column logic
    while (apartment.getColumn()->getLock() || apartment.getDashboard()->getLock())
      ;
    apartment.getColumn()->lock();
    apartment.getDashboard()->lock();

    // Panic Button
    if (apartment.getColumn()->getButton() && apartment.getColumn()->timer.finished())
    {
      // apartment.getDashboard()->setPanicAlert(true);
      apartment.getColumn()->setLed(true);
      apartment.getColumn()->timer.start();
    }
    // Fire alarm
    if (apartment.getColumn()->getSmokeSensor() > 700 && apartment.getColumn()->timer.finished() && apartment.getDashboard()->getFireAlert() == false)
    {
      apartment.getColumn()->timer.start(4);
      apartment.getDashboard()->setFireAlert(true);
      apartment.getColumn()->setBuzzer(true);
    }

    // Force the door open when the sensor detects an fire.
    while (apartment.getDoor()->getLock())
      ;

    apartment.getDoor()->lock();
    if (apartment.getColumn()->getSmokeSensor() > 700)
      apartment.getDoor()->setDoor(180);

    // Doorbel
    if (apartment.getDoor()->getButtonOut())
    {
      apartment.getColumn()->timer.start();
      apartment.getColumn()->setBuzzer(true);
    }

    // Unlock the door because we don't need it anymore.
    apartment.getDoor()->unlock();

    // Reset column outputs when timer is finished.
    if (apartment.getColumn()->timer.finished())
    {
      apartment.getColumn()->setBuzzer(false);
      apartment.getColumn()->setLed(false);
    }
    apartment.getColumn()->unlock();
    apartment.getDashboard()->unlock();

    // Bed Logic
    while (apartment.getBed()->getLock())
      ;
    apartment.getBed()->lock();
    if ((apartment.getBed()->getps() >= 100) && isNightTime(current))
    {
      apartment.getBed()->switchPast = apartment.getBed()->switchCurrent;

      if (apartment.getBed()->getsw())
      {
        apartment.getBed()->switchCurrent = apartment.getBed()->getsw();
      }
      else if (!apartment.getBed()->getsw())
      {
        apartment.getBed()->switchCurrent = apartment.getBed()->getsw();
      }
      if (!apartment.getBed()->switchCurrent && apartment.getBed()->switchPast)
      {
        apartment.getBed()->setled(!apartment.getBed()->getLed());
        apartment.getBed()->timer.start();
      }
      if (apartment.getBed()->timer.finished())
      {
        apartment.getBed()->setled(0);
      }
    }
    else if (!isNightTime(current))
    {
      apartment.getBed()->setled(0);
    }
    else if (!apartment.getBed()->timer.finished())
    {
      apartment.getBed()->timer.start();
    }
    apartment.getBed()->unlock();

    // Motion alert at nighttime logic
    while (apartment.getBed()->getLock() || apartment.getTableLamp()->getLock() || apartment.getDashboard()->getLock())
      ;
    apartment.getBed()->lock();
    apartment.getTableLamp()->lock();
    apartment.getDashboard()->lock();

    // If someone is laying on the bed, and it is nighttime, and we detect movement. Set a motion alert.
    if (apartment.getBed()->getps() > 100 && isNightTime(current) && apartment.getTableLamp()->getPirSensor())
    {
      apartment.getDashboard()->setMotionAlert(true);
    }

    apartment.getBed()->unlock();
    apartment.getTableLamp()->unlock();
    apartment.getDashboard()->unlock();

    // Chair logic
    while (apartment.getChair()->getLock())
      ;

    apartment.getChair()->lock();
    if (apartment.getChair()->getSwitch() && apartment.getChair()->getFsensor() > 100 && apartment.getChair()->timer.finished())
    {
      apartment.getChair()->switchCurrent = !apartment.getChair()->switchCurrent;
      apartment.getChair()->timer.start();
    }

    if (apartment.getChair()->getFsensor() <= 100)
    {
      apartment.getChair()->switchCurrent = false;
    }

    apartment.getChair()->setLed(apartment.getChair()->switchCurrent);
    apartment.getChair()->setMotor(apartment.getChair()->switchCurrent);
    apartment.getChair()->unlock();

    // Wall logic
    while (apartment.getWall()->getLock())
      ;
    apartment.getWall()->lock();

    // Auto dimming
    if (apartment.getWall()->getLightSen() <= 600 && isNightTime(current))
    {
      apartment.getWall()->setShadePan(1);
    }
    else
    {
      apartment.getWall()->setShadePan(0);
    }

    // Led Strip
    apartment.getWall()->setLedStrip(apartment.getWall()->getPotMeter() / 4);
    apartment.getWall()->unlock();
  }
}

void dashboard()
{
  sleep(10);
  while (1)
  {
    std::cout << "Waiting for connection\n";
    // Wait for client (blocking)
    int socket_fd = apartment.getServer().awaitClient();

    std::cout << "Connected!\n";

    int receiveStatus = 0;

    // Disconnect after message failure.
    while (receiveStatus >= 0)
    {
      std::cout << "Waiting for message!\n";
      // Wait for message (blocking)
      char buffer[4096] = {0};
      receiveStatus = apartment.getServer().receive(socket_fd, buffer, 4096);

      // Parse a JSON string into DOM.
      rapidjson::Document document;
      document.Parse(buffer);

      if (!document.IsObject())
      {
        std::cout << "Didn't recieve JSON! Closing connection\n";
        receiveStatus = -1;
        break;
      }

      while (apartment.getDashboard()->getLock())
        ;
      apartment.getDashboard()->lock();

      if (document.HasMember("openDoor") && document["openDoor"].IsBool())
      {
        apartment.getDashboard()->setDoor(document["openDoor"].GetBool());
      }

      if (document.HasMember("lampColor") && document["lampColor"].IsInt())
      {
        apartment.getDashboard()->setLampColor(document["lampColor"].GetInt());
      }

      if (document.HasMember("motionAlert") && document["motionAlert"].IsBool())
      {
        apartment.getDashboard()->setMotionAlert(document["motionAlert"].GetBool());
      }

      if (document.HasMember("fireAlert") && document["fireAlert"].IsBool())
      {
        apartment.getDashboard()->setFireAlert(document["fireAlert"].GetBool());
      }

      if (document.HasMember("fnt") && document["fnt"].IsBool())
      {
        apartment.getDashboard()->setForceNightTime(document["fnt"].GetBool());
      }

      std::cout << "Recieved!  " << buffer << "\n";

      apartment.getServer().send(socket_fd, apartment.getDashboard()->getJSON().c_str());
      apartment.getDashboard()->unlock();
    }

    close(socket_fd);

    receiveStatus = 0;
  }
}