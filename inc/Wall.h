#ifndef WALL_H
#define WALL_H

#include "Client.h"
#include "Module.h"

class Wall : public Module
{
private:
  int shadePan;
  int lightSen;
  int potMeter;
  int ledStrip = 255;

public:
  Wall();
  Wall(Client client);
  void setShadePan(bool val)
  {
    shadePan = val;
  }
  int getLightSen()
  {
    return lightSen;
  }
  int getPotMeter()
  {
    return potMeter;
  }
  void setLedStrip(int brightness)
  {
    ledStrip = brightness;
  }

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};

#endif