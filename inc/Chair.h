#include "Client.h"
#include "Module.h"

class Chair : public Module
{
private:
  bool motor = false;
  bool led = false;
  bool switch0 = false;
  int fsensor = 0;

public:
  Chair();
  Chair(Client client);

  bool switchCurrent;

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;

  // this will set the status of the motor
  Chair &setMotor(bool state)
  {
    motor = state;
    return *this;
  }

  // this will set the status of the led
  Chair &setLed(bool state)
  {
    led = state;
    return *this;
  }

  // this shows the current status of the switch
  bool getSwitch()
  {
    return switch0;
  }

  // this shows the value of the Forcesensor
  int getFsensor()
  {
    return fsensor;
  }
};
