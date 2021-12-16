#include "ZuilModule.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

ZuilModule::ZuilModule() : Module(Client()) {}
ZuilModule::ZuilModule(Client client) : Module(client) {}

void ZuilModule::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->knop = document["knop"].GetBool();
  this->rookSensor = document["rkSens"].GetBool();
}

std::string ZuilModule::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("led", this->led, allocator);
  document.AddMember("zoem", this->zoemer, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
