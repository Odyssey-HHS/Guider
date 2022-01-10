#ifndef MODULE_H
#define MODULE_H

#include <string>
#include "Client.h"

class Module
{
public:
    Module(Client client);
    virtual std::string getOutputsJSON() const = 0;
    virtual std::string getInputsJSON() const = 0;
    virtual void setInputsJSON(const std::string json) = 0;

    /* Sync this object with the Wemos module. Writes to the outputs and reads from the inputs of the Wemos */
    void fetch();

    /* Lock this object so other threads wait for an unlock */
    void lock();

    /* Unlock this object so other threads can use it again. */
    void unlock();

    /* Get the current lock status of this object. Returns an true on lock. */
    bool getLock();

    void setUpdated(bool);

    bool getUpdated(); 

private:
    Client connection;
    bool lockFile = false;
    bool updated = false;
};
#endif // MODULE_H