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

  this->buttonIn = document["btnI"].GetBool();
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