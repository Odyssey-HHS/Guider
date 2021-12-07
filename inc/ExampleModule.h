#include "Client.h"
#include "Module.h"

class ExampleModule : public Module
{
public:
  ExampleModule();
  ExampleModule(Client client);
  bool led0 = false;
  bool led1 = false;
  bool button0 = false;
  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};