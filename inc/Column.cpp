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

  if (!document.IsObject())
    return;

  if (document.HasMember("btn") && document["btn"].IsBool())
    this->button = document["btn"].GetBool();

  if (document.HasMember("smk") && document["smk"].IsInt())
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

bool Column::getLed()
{
  return this->led;
}

int Column::getSmokeSensor()
{
  return this->smokeSensor;
}

Column &Column::setLed(bool val)
{
  led = val;
  return *this;
}

Column &Column::setBuzzer(bool val)
{
  buzzer = val;
  return *this;
}
