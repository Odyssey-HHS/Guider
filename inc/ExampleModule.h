#include "Client.h"
#include "Module.h"

class ExampleModule : public Module
{
public:
  ExampleModule();
  ExampleModule(Client client);
  bool ledIn = false;
  bool ledOut = false;
  int door = 0;

  bool buttonOut = false;
  bool buttonIn = false;

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};