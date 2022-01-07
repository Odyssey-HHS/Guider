#include "Dashboard.h"

bool Dashboard::getDoor()
{
    return this->openDoor;
}

int Dashboard::getLampColor()
{
    return this->lampColor;
}

Dashboard &Dashboard::setDoor(bool val)
{
    this->openDoor = val;
    return *this;
}

Dashboard &Dashboard::setLampColor(int val)
{
    this->lampColor = val;
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