#include "Client.h"
#include "Module.h"

class Bed : public Module
{
private:
  bool Led;
  bool Switch;
  int DB; // druk sensor dit is analoog
public:
  Bed();
  Bed(Client client);
  int setLed();    // this will set the status of the led
  int getSwitch(); // this shows the current status of the switch
  int getDB();     // this shows the output value of the druksensor

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};