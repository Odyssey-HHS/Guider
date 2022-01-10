#include <iostream>
#include "Chair.h"
#include "ModuleAddresses.h"
#include <thread>

// Declair an instance of the module
Chair chair;

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

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
    chair.fetch();

    // Sleep for a bit because we only have one module and we don't want to overload it.
    usleep(100000);
  }
}

/* Execute logic functions, these manipulate the outputs of modules. */
void logic()
{
  while (1)
  {

    if (chair.getSwitch())
    {
      chair.switchCurrent != chair.switchCurrent;
    }

    chair.lock();
    chair.setLed(false);
    chair.unlock();

    chair.lock();
    chair.setMotor(false);
    chair.unlock();

    while (chair.switchCurrent)
    {

      // print value force sensor
      std::cout << "ForceSensor: " << chair.getFsensor() << "\n";

      // print value push button
      std::cout << "button: " << chair.getSwitch() << "\n";

      //
      chair.lock();
      chair.setLed(true);
      chair.unlock();

      chair.lock();
      chair.setMotor(true);
      chair.unlock();
    }
  }
}