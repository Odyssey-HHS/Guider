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

  this->button = document["btn"].GetBool();
  this->smokeSensor = document["smk"].GetInt();
}

std::string Column::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("ld", this->led, allocator);
  document.AddMember("bz", this->buzzer, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}

bool Column::getButton()
{
  return this->button;
}

void Column::setBuzzer(bool input)
{
  this->buzzer = input;
}

void Column::setLed(bool input)
{
  this->led = input;
}
