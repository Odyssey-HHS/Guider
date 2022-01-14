#include "Client.h"
#include "Module.h"

class wall : public Module
{
private:
int shadePan;
int lightSen;
int potMeter;
int ledStrip = 255;

public:
  wall();
  wall(Client client);
  void setshadePan(bool val)
  {
      shadePan = val;
  }
  int getlightsen()
  {
      return lightSen;
  }
  int getpotMeter()
  {
      return potMeter;
  }
  void setledStrip(int brightness)
  {
    ledStrip = brightness;
  }

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};