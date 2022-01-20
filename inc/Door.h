#ifndef DOOR_H
#define DOOR_H

#include "Client.h"
#include "Module.h"
#include <iostream>

class Door : public Module
{
public:
    Door();
    Door(Client client);

    void setInputsJSON(const std::string json);
    std::string getOutputsJSON() const;

    Door &setLedIn(bool val);
    Door &setLedOut(bool val);
    Door &setDoor(int val);
    bool getLedIn() { return ledIn; }
    bool getLedOut() { return ledOut; }
    int getDoor() { return door; }
    bool getButtonIn() { return buttonIn; }
    bool getButtonOut() { return buttonOut; }

private:
    bool ledIn = false;
    bool ledOut = false;
    int door = 0;

    bool buttonOut = false;
    bool buttonIn = false;
};
#endif // DOOR_H