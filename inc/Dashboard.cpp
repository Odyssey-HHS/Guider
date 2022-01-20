#include "Dashboard.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

std::string Dashboard::getJSON() const
{
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("door", this->openDoor, allocator);
  document.AddMember("lampColor", this->lampColor, allocator);
  document.AddMember("motionAlert", this->motionAlert, allocator);
  document.AddMember("fireAlert", this->fireAlert, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}

bool Dashboard::getDoor()
{
  return this->openDoor;
}

bool Dashboard::getFireAlert()
{
  return this->fireAlert;
}

int Dashboard::getLampColor()
{
  return this->lampColor;
}

bool Dashboard::getForceNightTime()
{
  return this->forceNightTime;
}

Dashboard &Dashboard::setDoor(bool val)
{
  this->openDoor = val;
  return *this;
}

Dashboard &Dashboard::setForceNightTime(bool val)
{
  this->forceNightTime = val;
  return *this;
}

Dashboard &Dashboard::setLampColor(int val)
{
  this->lampColor = val;
  return *this;
}

Dashboard &Dashboard::setMotionAlert(bool val)
{
  this->motionAlert = val;
  return *this;
}

Dashboard &Dashboard::setFireAlert(bool val)
{
  this->fireAlert = val;
  return *this;
}

Dashboard &Dashboard::setPanicAlert(bool val)
{
  this->panicAlert = val;
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