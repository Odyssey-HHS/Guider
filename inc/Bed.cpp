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

  this->Led = document["LedB"].GetBool();
  this->Switch = document["SwB"].GetBool();
  this->DB = document["DBV"].GetInt64();
}

std::string Bed::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("LedB", this->Led, allocator);
  document.AddMember("SwichtB", this->Switch, allocator);
  document.AddMember("DBB", this->DB, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}