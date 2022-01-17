#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <string>

class Dashboard
{
public:
    bool getDoor();
    int getLampColor();
    Dashboard &setDoor(bool);
    Dashboard &setLampColor(int);
    Dashboard &setMotionAlert(bool);
    std::string getJSON() const;
    Dashboard &update();
    bool hasChanged();

    bool getLock();
    void lock();
    void unlock();

private:
    bool openDoor = false;
    int lampColor = 0;
    bool lockFile = false;
    bool motionAlert = false;
    bool changed = false;
};
#endif