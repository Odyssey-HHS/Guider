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

std::string Column::getInputsJSON() const
{
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("btn", this->button, allocator);
  document.AddMember("smk", this->smokeSensor, allocator);

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

bool Column::getBuzzer()
{
  return this->buzzer;
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
  if (led == val)
  {
    setUpdated(true);
  }
  led = val;
  return *this;
}

Column &Column::setBuzzer(bool val)
{
  if (buzzer == val)
  {
    setUpdated(true);
  }
  buzzer = val;
  return *this;
}
