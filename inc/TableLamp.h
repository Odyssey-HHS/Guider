#include "Client.h"
#include "Module.h"

struct RGBLed {
  int red;
  int green;
  int blue;
};

class TableLamp : public Module
{
public:
  TableLamp();
  TableLamp(Client client);

  bool pirSensor = false;
  struct RGBLed led = {0, 0, 0};

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};