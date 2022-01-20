#include "Module.h"

Module::Module(Client client) : connection(client){};

void Module::fetch()
{
  // Wait for unlock
  while (getLock())
    ;

  lock();
  // Get the JSON data containing the module outputs.
  const std::string outputs = getOutputsJSON();
  unlock();

  // Write the JSON output data to the wemos module, returning the json input data.
  this->connection.send(outputs.c_str());
  char buffer[256] = {0};
  this->connection.receive(buffer, 256);

  // Wait for unlock
  while (getLock())
    ;

  // Update the testModule object with the new input data.
  lock();
  setInputsJSON(buffer);
  unlock();
}

void Module::lock()
{
  this->lockFile = true;
}

void Module::unlock()
{
  this->lockFile = false;
}

bool Module::getLock()
{
  return this->lockFile;
}