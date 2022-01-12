#include <iostream>

#include "Column.h"
#include "ModuleAddresses.h"
#include <thread>
#include <string>

using namespace std;
// Declair an instance of the module
Column column;

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
   std::cout << "Connecting to Column.."
            << "\n";
  Client columnClient(COLUMN_MODULE, 8080);

  // Create a new module using the connection created above.
  column = Column(columnClient);

  // Spin up the two threads.
  std::thread fetcherThread(fetcher);
  std::thread logicThread(logic);

  // Close down the threads when they are finished.
  fetcherThread.join();
  logicThread.join();
}

// Updates the module objects syncing them with the wemos hardware.
void fetcher()
{
  while (1)
  {
    // Synchronize the object with the Wemos module
    std::cout << "Fetching Column...\n";
    column.fetch();

    std::cout << "Starting new fetching round...\n";
  }
}

// Execute logic functions, these manipulate the outputs of modules.
void logic()
{
  while (1)
  {
    // Buzzer gaat aan zodra er op de knop wordt gedrukt.
    if (column.getButton())
    {
      cout << "DE ALARMKNOP IS INGEDRUKT. DE BEWONER IS IN NOOD" << endl;
    }
  }
}