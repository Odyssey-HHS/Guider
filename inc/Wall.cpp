#include "Wall.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

wall::wall() : Module(Client()) {}
wall::wall(Client client) : Module(client) {}

void wall::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->lightSen = document["ldr"].GetInt();
  this->potMeter = document["ptm"].GetInt();
}

std::string wall::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("lcd", this->shadePan, allocator);
  document.AddMember("rgb", this->ledStrip, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  

  std::string output = buffer.GetString();
  return output;
}
