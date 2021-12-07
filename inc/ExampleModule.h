#include "Client.h"
#include "Module.h"

class ExampleModule : public Module
{
public:
  ExampleModule(Client client);
  bool led0 = false;
  bool led1 = false;
  bool button0 = false;
  void setInputsJSON(const char *json);
  std::string getOutputsJSON() const;
};