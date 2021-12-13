#include <Wire.h>
#include <ESP8266WiFi.h>

class DigitalIO
{
public:
    DigitalIO(const int icAddress);
    unsigned int readInput();
    void setOutput(const int pin, const bool state);

private:
    void configureIC(const int address);
    int icAddress;
    int inAddress;
    int outAddress;
};
