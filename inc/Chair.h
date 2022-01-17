#include "Client.h"
#include "Module.h"

class Chair : public Module
{
private:
  bool Motor = false;
  bool Led = false;
  bool Switch = false;
  int Fsensor = 0;

public:
  Chair();
  Chair(Client client);

  bool switchPast;
  bool switchCurrent;

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;

  // this will set the status of the motor
  Chair &setMotor(bool state)
  {
    Motor = state;
    return *this;
  }

  // this will set the status of the led
  Chair &setLed(bool state)
  {
    Led = state;
    return *this;
  }

  // this shows the current status of the switch
  bool getSwitch()
  {
    return Switch;
  }

  // this shows the value of the Forcesensor
  int getFsensor()
  {
    return Fsensor;
  }
};
