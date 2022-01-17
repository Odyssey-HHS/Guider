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
    this->changed = true;
    this->openDoor = val;
    return *this;
}

Dashboard &Dashboard::setForceNightTime(bool val)
{
    this->changed = true;
    this->forceNightTime = val;
    return *this;
}

Dashboard &Dashboard::setLampColor(int val)
{
    this->changed = true;
    this->lampColor = val;
    return *this;
}

Dashboard &Dashboard::setMotionAlert(bool val)
{
    this->changed = true;
    this->motionAlert = val;
    return *this;
}

Dashboard &Dashboard::update()
{
    this->changed = false;
    return *this;
}

bool Dashboard::hasChanged()
{
    return this->changed;
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