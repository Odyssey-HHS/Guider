#include "Client.h"
#include "Module.h"

class Column : public Module
{
private:
  bool led = false; // de led in de knop
  bool buz = false; // de buzzer
  bool smkS = false; // de rooksensor
  bool btn = false; // de knop op de zuil

public:
  Column();
  Column(Client client);

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
};