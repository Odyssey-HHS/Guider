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
  TableLamp &setLed(const uint8_t red, const uint8_t green, const uint8_t blue);

private:
  bool pirSensor = false;
  int led = 0;
};