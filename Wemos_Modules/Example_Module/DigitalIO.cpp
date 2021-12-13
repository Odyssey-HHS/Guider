/*
  DigitalIO.cpp - Library for using digital IO ports of the WIB Board.
  Created by Group 1 PJSDV, December 13, 2021.
  Released into the public domain.
*/

#include "DigitalIO.h"

DigitalIO::DigitalIO(const int icAddress) : icAddress(icAddress), inAddress(0), outAddress(1)
{
    this->configureIC(icAddress);
}

void DigitalIO::configureIC(const int address)
{
    Wire.beginTransmission(address); // Choose the PCA9554A
    Wire.write(byte(0x03));          // DDR register Hex address 0X03 to choose  input or output
    Wire.write(byte(0x0F));          // 00001111   DIO0-DIO3 = INPUTS    DIO4-DIO7 = OUTPUTS
    Wire.endTransmission();          // End I2C connection
}

void DigitalIO::setOutput(const int pin, const bool state)
{
    int value = state;

    Wire.beginTransmission(this->icAddress); // Choose the PCA9554A
    Wire.write(byte(this->outAddress));      // Hex adress 0x01 to set outputs (DIO4-DIO7) to 1 or 0
    Wire.write(byte(value << pin));         // Set one of the output (DIO4-DIO7) to 1 or 0
    Wire.endTransmission();                 // End I2C connection
}

unsigned int DigitalIO::readInput()
{
    Wire.beginTransmission(this->icAddress); // Choose the PCA9554A
    Wire.write(byte(this->inAddress));       // Hex adress 0x00 to read inputs (DIO0-DIO3)
    Wire.endTransmission();                 // End I2C connection
    Wire.requestFrom(this->inAddress, 1);    // Request values from PCA9554A , 1 Byte

    unsigned int inputs = Wire.read(); // Copy values to variable inputs
    return (inputs & 0x0f);
}
