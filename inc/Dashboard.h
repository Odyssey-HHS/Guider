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
    Dashboard &setForceNightTime(bool);
    std::string getJSON() const;
    Dashboard &update();
    bool hasChanged();
    bool getForceNightTime();

    bool getLock();
    void lock();
    void unlock();

private:
    bool openDoor = false;
    bool forceNightTime = false;
    int lampColor = 0;
    bool lockFile = false;
    bool motionAlert = false;
    bool changed = false;
};
#endif