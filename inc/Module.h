#include <string>
#include "Client.h"

class Module
{
public:
    Module(Client client);
    virtual std::string getOutputsJSON() const = 0;
    virtual void setInputsJSON(const std::string json) = 0;

    /* Sync this object with the Wemos module. Writes to the outputs and reads from the inputs of the Wemos */
    std::string fetch(const std::string outputJson);

private:
    Client connection;
};