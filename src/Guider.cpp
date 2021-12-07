#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include "Module.h"

class ExampleModule : public Module
{
public:
  ExampleModule(Client client);
  int led0 = -1;
  int led1 = -1;
  int button0 = -1;
  void setInputsJSON(const char *json);
  char *getOutputsJSON() const;
};

ExampleModule::ExampleModule(Client client) : Module(client) {}

void ExampleModule::setInputsJSON(const char *json)
{
  // Grep the input values from the JSON and store it in this class.
  std::cout << "Not implemented\n";
  return;
}

char *ExampleModule::getOutputsJSON() const {
  // Grep the outputs of this class and return it as JSON
  std::cout << "Not implemented\n";
  return {0};
}

int main(int argc, char const *argv[])
{
  Client client("172.16.99.100", 8080);
  ExampleModule module(client);

  std::cout << "test: " << module.led0;
}