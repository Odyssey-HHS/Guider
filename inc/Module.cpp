#include "Module.h"

Module::Module(Client client) : connection(client){};

void Module::fetch()
{
    this->connection.send(this->getOutputsJSON().c_str());
    char buffer[256] = {0};
    this->connection.receive(buffer, 256);

    this->setInputsJSON(buffer);
}