#include <string>
#include "Client.h"

class Module
{
public:
    Module(Client client);
    virtual std::string getOutputsJSON() const = 0;
    virtual void setInputsJSON(const char *json) = 0;

    /* Sync this object with the Wemos module. Writes to the outputs and reads from the inputs of the Wemos */
    void fetch();

private:
    Client connection;
};