#include "ExampleModule.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

ExampleModule::ExampleModule(Client client) : Module(client) {}

void ExampleModule::setInputsJSON(const char *json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json);

  this->button0 = document["button0"].GetBool();
}

std::string ExampleModule::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  document.AddMember("led0", this->led0, allocator);
  document.AddMember("led1", this->led1, allocator);

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}
