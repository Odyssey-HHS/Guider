#include "Client.h"
#include "Module.h"
#include <iostream>

class Column : public Module
{
private:
  bool led = false;    // de led in de knop
  bool buzzer = false; // de buzzer
  int smokeSensor = 0; // de rooksensor
  bool button = false; // de knop op de zuil

public:
  Column();
  Column(Client client);

  void setInputsJSON(const std::string json);
  std::string getOutputsJSON() const;
  std::string getInputsJSON() const;

  bool getButton();
  int getSmokeSensor();
  bool getLed();

  Column &setBuzzer(bool val);
  Column &setLed(bool val);
};