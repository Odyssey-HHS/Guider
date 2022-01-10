#include "Client.h"
#include "Module.h"

class wall : public Module
{
private:
int shadePan;
int lightSen;
int potMeter;
int ledStrip;

public:
  wall();
  wall(Client client);
  void setlcd(bool val)
  {
      shadePan = val;
  }
  int getshadepanal()
  {
      return lightSen;
  }
  int getpot()
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