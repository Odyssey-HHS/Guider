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
  Dashboard &setFireAlert(bool);
  Dashboard &setPanicAlert(bool);
  Dashboard &setForceNightTime(bool);
  std::string getJSON() const;
  bool getForceNightTime();
  bool getFireAlert();
  bool getLock();
  void lock();
  void unlock();

private:
  bool openDoor = false;
  bool forceNightTime = false;
  int lampColor = 0;
  bool lockFile = false;
  bool motionAlert = false;
  bool fireAlert = false;
  bool panicAlert = false;
};
#endif