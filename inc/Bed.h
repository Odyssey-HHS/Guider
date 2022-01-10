#include "Client.h"
#include "Module.h"

class Bed : public Module
{
private:
  bool led;
  bool sw;
  int ps; // Pressure sensor

public:
  Bed();
  Bed(Client client);
  bool swp;                // button state past
  bool swc;                // button state current
  void setled(bool newVal) // this will set the status of the led
  {
    led = newVal;
  }
  bool getsw() // this shows the current status of the switch
  {
    return sw;
  }
  int getps() // this shows the output value of the presuresensor
  {
    return ps;
  }
  int getLed() // this returns the status of the LED
  {
    return led;
  }

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};