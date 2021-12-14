#include "TableLamp.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

TableLamp::TableLamp() : Module(Client()) {}
TableLamp::TableLamp(Client client) : Module(client) {}

void TableLamp::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->pirSensor = document["pir"].GetBool();
}

std::string TableLamp::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("rgb0", this->led, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
