#ifndef TABLELAMP_H
#define TABLELAMP_H

#include "Client.h"
#include "Module.h"
#include "Timer.h"

class TableLamp : public Module
{
public:
  TableLamp();
  TableLamp(Client client);

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;

  Timer timer = Timer(2);

  bool getPirSensor();
  TableLamp &setLed(const int red, const int green, const int blue);
  TableLamp &setLed(const int rgb);

private:
  bool pirSensor = false;
  int led = 0;
};
#endif // TABLELAMP_H