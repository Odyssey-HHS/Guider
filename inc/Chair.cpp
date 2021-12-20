#include "Chair.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Chair::Chair() : Module(Client()) {}
Chair::Chair(Client client) : Module(client) {}

void Chair::setInputsJSON(const std::string json)
{
  // 1. Parse a JSON string into DOM.
  rapidjson::Document document;
  document.Parse(json.c_str());

  this->Motor = document["vM"].GetBool();
  this->Led = document["lChr"].GetBool();
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
  document.AddMember("btnC", this->Switch, allocator);
  document.AddMember("fsCh", this->Fsensor, allocator);
  

  // Stringify object
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string output = buffer.GetString();
  return output;
}


bool Chair::setMotor(bool state)
{
this->Motor = state;
//return *this; 
}

bool Chair::setLed(bool state)
{
this->Led = state;       
//return *this;
}


bool Chair::getSwitch()
{
return this->Switch;
}


int Chair::getFsensor()
{
return this->Fsensor;

}

