#include "Module.h"

Module::Module(Client client) : connection(client){};

std::string Module::fetch(const std::string outputJson)
{
    this->connection.send(outputJson.c_str());
    char buffer[256] = {0};
    this->connection.receive(buffer, 256);
    return std::string(buffer);
}