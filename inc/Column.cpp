#include "Column.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Column::Column() : Module(Client()) {}
Column::Column(Client client) : Module(client) {}

void Column::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->btn = document["btn"].GetBool();
  this->smkS = document["smkS"].GetBool();
}

std::string Column::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("led", this->led, allocator);
  document.AddMember("buz", this->buz, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
