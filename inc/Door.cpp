#include "Door.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Door::Door() : Module(Client()) {}
Door::Door(Client client) : Module(client) {}

void Door::setInputsJSON(const std::string json)
{
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (!document.IsObject()) return;

  if (document.HasMember("btnI") && document["btnI"].IsBool())
    this->buttonIn = document["btnI"].GetBool();

  if (document.HasMember("btnO") && document["btnO"].IsBool())
    this->buttonOut = document["btnO"].GetBool();
}

std::string Door::getOutputsJSON() const
{
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("ledI", this->ledIn, allocator);
  document.AddMember("ledO", this->ledOut, allocator);
  document.AddMember("door", this->door, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}

std::string Door::getInputsJSON() const
{
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("btnI", this->buttonIn, allocator);
  document.AddMember("btnO", this->buttonOut, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}

Door &Door::setLedIn(bool val)
{
  if (ledIn == val)
  {
    setUpdated(true);
  }
  ledIn = val;
  return *this;
}
Door &Door::setLedOut(bool val)
{
  if (ledOut == val)
  {
    setUpdated(true);
  }
  ledOut = val;
  return *this;
}
Door &Door::setDoor(int val)
{
  if (door == val)
  {
    setUpdated(true);
  }
  door = val;
  return *this;
}