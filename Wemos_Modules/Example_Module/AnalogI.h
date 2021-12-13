#include <Wire.h>
#include <ESP8266WiFi.h>

class AnalogI
{
public:
    AnalogI(const int icAddress);
    unsigned int readAnalogInput(int ANALOG_CH);
    

private:
    void configureAnalogIC(const int address);
    int icAddress;

};
