#include "Client.h"
#include "Module.h"

class ZuilModule : public Module
{
private:
  bool led = false;
  bool zoemer = false;
  bool rookSensor = false;
  bool knop = false;

public:
  ZuilModule();
  ZuilModule(Client client);

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};