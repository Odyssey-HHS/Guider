#include "Client.h"
#include "Module.h"


class Chair : public Module
{
private:
  bool Motor;
  bool Led;
  bool Switch;
  int Fsensor;

public:
  Chair();
  Chair(Client client);

  Bed(Client client);
  bool setMotor();      // this will set the status of the motor
  bool setLed();        // this will set the status of the led
  bool getSwitch();     // this shows the current status of the switch
  int getFsensor();     // this shows the value of the Forcesensor

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
}; 

