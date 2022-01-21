#include "Wall.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Wall::Wall() : Module(Client()) {}
Wall::Wall(Client client) : Module(client) {}

void Wall::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (!document.IsObject())
    return;

  if (document.HasMember("ldr") && document["ldr"].IsInt())
    this->lightSen = document["ldr"].GetInt();

  if (document.HasMember("ptm") && document["ptm"].IsInt())
    this->potMeter = document["ptm"].GetInt();
}

std::string Wall::getOutputsJSON() const
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
