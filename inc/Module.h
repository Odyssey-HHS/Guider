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

    /* Lock this object so other threads wait for an unlock */
    void lock();

    /* Unlock this object so other threads can use it again. */
    void unlock();

    /* Get the current lock status of this object. Returns an true on lock. */
    bool getLock();

private:
    Client connection;
    bool lockFile;
};