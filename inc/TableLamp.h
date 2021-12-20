#include "Client.h"
#include "Module.h"

class TableLamp : public Module
{
public:
  TableLamp();
  TableLamp(Client client);

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;

  bool getPirSensor();
  TableLamp &setLed(const int red, const int green, const int blue);

private:
  bool pirSensor = false;
  int led = 0;
};