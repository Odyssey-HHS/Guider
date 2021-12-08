#include <iostream>

#include "ExampleModule.h"
#include <thread>

ExampleModule testMod;
bool testModLocked = false;

void fetcher()
{
  while (1)
  {
    while (testModLocked)
      ;
    testModLocked = true;
    const std::string outputs = testMod.getOutputsJSON();
    testModLocked = false;

    // This takes a "long" time.
    const std::string inputs = testMod.fetch(outputs.c_str());

    while (testModLocked)
      ;
    testModLocked = true;
    testMod.setInputsJSON(inputs.c_str());
    testModLocked = false;
    usleep(100000); // But of sleep because we only have one module
  }
}

void logic()
{
  while (1)
  {
    while (testModLocked)
      ;
      if (testMod.buttonIn) {
        testMod.door = 0;
      } else if (testMod.buttonOut) {
        testMod.door = 180;
      }
      else {
        testMod.door = 65;
      }
  }
}

int main(int argc, char const *argv[])
{
  Client client("172.16.99.100", 8080);
  testMod = ExampleModule(client);

  std::cout << "Up and running!";

  std::thread fetcherThread(fetcher);
  std::thread logicThread(logic);

  fetcherThread.join();
  logicThread.join();
}