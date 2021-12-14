#include <iostream>

#include "ExampleModule.h"
#include "ModuleAddresses.h"
#include <thread>

// Declair an instance of the module
ExampleModule testModule;

// Declair the two functions used in seperate threads.
void fetcher();
void logic();

// The main function, creates the connections to the modules and spins up the threads.
int main(int argc, char const *argv[])
{
  // Create a new connection to the Wemos board.
  Client client(EXAMPLE_MODULE, 8080);

  // Create a new module using the connection created above.
  testModule = ExampleModule(client);

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
    // Wait for unlock
    while (testModule.getLock())
      ;

    testModule.lock();
    // Get the JSON data containing the module outputs.
    const std::string outputs = testModule.getOutputsJSON();
    testModule.unlock();

    // Write the JSON output data to the wemos module, returning the json input data.
    const std::string inputs = testModule.fetch(outputs.c_str());

    // Wait for unlock
    while (testModule.getLock())
      ;

    // Update the testModule object with the new input data.
    testModule.lock();
    testModule.setInputsJSON(inputs.c_str());
    testModule.unlock();

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
    while (testModule.getLock())
      ;
    if (testModule.buttonIn)
    {
      testModule.door = 0;
    }
    else if (testModule.buttonOut)
    {
      testModule.door = 180;
    }
    else
    {
      testModule.door = 65;
    }
  }
}