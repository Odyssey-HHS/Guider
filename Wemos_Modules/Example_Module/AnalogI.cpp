/*
  AnalogI.cpp - Library for using analog I ports of the WIB Board.
  Created by Group 1 PJSDV, December 13, 2021.
  Released into the public domain.
*/

#include "AnalogI.h"


AnalogI::AnalogI(const int icAddress) : icAddress(icAddress)
{
    this->configureAnalogIC(icAddress);
}


/* Config MAX11647 Analog inputs */
void AnalogI::configureAnalogIC(const int address)
{
    Wire.beginTransmission(address); // Choose the MAX11647
    Wire.write(byte(0xA2));                 // set-up byte
    Wire.write(byte(0x03));                 // configuration byte
    Wire.endTransmission();                 // End I2C connection
}


/* Read the analog channel of the MAX11647 */
unsigned int AnalogI::readAnalogInput(int ANALOG_CH)
{
    unsigned int anin0;
    unsigned int anin1;

    // Read MAX11647
    if (ANALOG_CH == 0)
    {
        Wire.requestFrom(icAddress, 4); // Request values from MAX11647 , 4 Bytes
        anin0 = Wire.read() & 0x03;          // AND values with 0000 0011 Copy values to variable anin0
        anin0 = anin0 << 8;                  // Shift anin0 8 places
        anin0 = anin0 | Wire.read();         // OR anin1 with data from analog ic
        return anin0;                        // Return value of anin0
    }

    if (ANALOG_CH == 1)
    {
        Wire.requestFrom(icAddress, 4); // Request values from MAX11647 , 4 Bytes
        anin1 = Wire.read() & 0x03;          // AND values with 0000 0011 Copy values to variable anin1
        anin1 = anin1 << 8;                  // Shift anin1 8 places
        anin1 = anin1 | Wire.read();         // OR anin1 with data from analog ic
        return anin1;                        // Return value of anin1
    }
}
