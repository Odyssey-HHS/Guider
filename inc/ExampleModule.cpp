#include "ExampleModule.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

ExampleModule::ExampleModule() : Module(Client()) {}
ExampleModule::ExampleModule(Client client) : Module(client) {}

void ExampleModule::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->buttonIn = document["buttonIn"].GetBool();
  this->buttonOut = document["buttonOut"].GetBool();
}

std::string ExampleModule::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  document.AddMember("ledIn", this->ledIn, allocator);
  document.AddMember("ledOut", this->ledOut, allocator);
  document.AddMember("door", this->door, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
