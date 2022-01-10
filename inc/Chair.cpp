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


  this->Switch = document["btnC"].GetBool();
  this->Fsensor = document["fsCh"].GetInt64();
}

std::string Chair::getOutputsJSON() const
{
  // Create JSON object.
  rapidjson::Document document;
  document.SetObject();

  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  document.AddMember("vM", this->Motor, allocator);
  document.AddMember("lChr", this->Led, allocator);

  

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}

