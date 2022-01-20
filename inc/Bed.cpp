#include "Bed.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

Bed::Bed() : Module(Client()) {}
Bed::Bed(Client client) : Module(client) {}

void Bed::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (!document.IsObject())
    return;

  if (document.HasMember("Sw") && document["Sw"].IsBool())
  {
    this->sw = document["Sw"].GetBool();
  }

  if (document.HasMember("PS") && document["PS"].IsInt64())
  {
    this->ps = document["PS"].GetInt64();
  }
}

std::string Bed::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("Led", this->led, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}