#include "Dashboard.h"

bool Dashboard::getDoor()
{
    return this->openDoor;
}

int Dashboard::getLampColour()
{
    return this->lampColour;
}

Dashboard &Dashboard::setDoor(bool val)
{
    this->openDoor = val;
    return *this;
}

Dashboard &Dashboard::setLampColour(int val)
{
    this->lampColour = val;
    return *this;
}

bool Dashboard::getLock()
{
    return this->lockFile;
}

void Dashboard::lock()
{
    this->lockFile = true;
}

void Dashboard::unlock()
{
    this->lockFile = false;

}