#include "Client.h"
#include "Module.h"

class Bed : public Module
{
private:
  bool Led;
  bool Switch;
  int DS; //druk sensor dit is analoog
public:
  Bed();
  Bed(Client client);
  int setLed();    //this will set the status of the led
  int getSw(); // this shows the current status of the switch
  int getDS();     //this shows the output value of the druksensor

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};