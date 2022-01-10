#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include "Bed.h"
#include "ModuleAddresses.h"
#include "Door.h"
#include "TableLamp.h"
<<<<<<< Updated upstream
#include <thread>
=======
#include "Bed.h"
#include "Wall.h"

>>>>>>> Stashed changes
#include "Timer.h"

// Declair an instance of the module
Bed bed;
<<<<<<< Updated upstream
//TableLamp tableLamp;
//Door door;
=======
TableLamp tableLamp;
Door door;
wall Wall;
>>>>>>> Stashed changes

//Timer doorLightTimer = Timer(5);
//Timer tableLampTimer = Timer(2);
Timer bedTimer = Timer(10);

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  
  // Create a new connection to the Wemos board.
  Client bedClient(BED_MODULE, 8080);
<<<<<<< Updated upstream
 // Client lampClient(LAMP_MODULE, 8080);
  //Client doorClient(DOOR_MODULE, 8080);

  // Create a new module using the connection created above.
  bed = Bed(bedClient);
  //tableLamp = TableLamp(lampClient);
  //door = Door(doorClient);
=======
  Client lampClient(LAMP_MODULE, 8080);
  Client doorClient(DOOR_MODULE, 8080);
  Client WallClient(WALL_MODULE, 8080);

  // Create a new module using the connection created above.
  bed = Bed(bedClient);
  tableLamp = TableLamp(lampClient);
  door = Door(doorClient);
  Wall = wall(WallClient);
>>>>>>> Stashed changes

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
    //tableLamp.fetch();
    //door.fetch();
    bed.fetch();
    Wall.fetch();

    // Sleep for a bit because we only have 2 module and we don't want to overload them.
    usleep(100000);
    //std::cout << "Fetching round...\n";
  }
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {
    std::time_t current = std::time(nullptr);

<<<<<<< Updated upstream
    // Example door logic, this is just an example and should be cleaned up for use with multiple modules.
    while (bed.getLock())
    ;
    if (bed.getps()>=100)
    {
      bed.swp = bed.swc;
            
      if(bed.getsw()==1)
      {
        bed.swc = bed.getsw();
      }
      if(bed.getsw()==0)
      {
        bed.swc = bed.getsw();
      }
      if(bed.swc == 0 && bed.swp == 1)
      {
        bed.setled(!bed.getLed());
        bedTimer.start();
      }
      if(bedTimer.finished())
      {
        bed.setled(0);
      }
    }
      // usleep(1000);
      // testModule.setled(0);
    /*while (tableLamp.getLock())
=======
    /*// Table Lamp Logic, turns white on motion, otherwise it runs to the dashboard provided color.
    while (tableLamp.getLock() || dashboardModule.getLock())
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    tableLamp.unlock();*/
  
  
=======
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
    dashboardModule.unlock();*/
    
    // logic Wall
    while (Wall.getLock())
      ;
    Wall.lock();
  }
>>>>>>> Stashed changes
}
}