#include "Bed.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Bed::Bed() : Module(Client()) {}
Bed::Bed(Client client) : Module(client) {}

void Bed::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->Led = document["Led"].GetBool();
  this->Switch = document["Sw"].GetBool();
  this->DS = document["DS"].GetInt64();
}

std::string Bed::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("Led", this->Led, allocator);
  document.AddMember("Swicht", this->Switch, allocator);
  document.AddMember("DS", this->DS, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}