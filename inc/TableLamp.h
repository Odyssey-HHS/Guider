#include "Client.h"
#include "Module.h"

struct RGBLed
{
  int red;
  int green;
  int blue;
};

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
  struct RGBLed led = {0, 0, 0};
};