#include "Chair.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

Chair::Chair() : Module(Client()) {}
Chair::Chair(Client client) : Module(client) {}

void Chair::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (!document.IsObject())
    return;

  if (document.HasMember("btnC") && document["btnC"].IsBool())
    this->switch0 = document["btnC"].GetBool();

  if (document.HasMember("fsCh") && document["fsCh"].IsInt64())
    this->fsensor = document["fsCh"].GetInt64();
}

std::string Chair::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("vM", this->motor, allocator);
  document.AddMember("lChr", this->led, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
