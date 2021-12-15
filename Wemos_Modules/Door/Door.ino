/*
 * HHS PJSD 2021
 *
 * Groep 1
 *
 * Marko Estrada Rodriguez
 * Wouter de Bruijn
 * Casper Bastiaan-Net
 * Maurice Tacx
 * Steven Cowan
 *
 * Wemos I/O Server
 *
 * Wemos module connecting to a wifi network and creating a TCP server.
 * Can receive TCP server commands to set or get data.
 * Can use IO from WIB Board
 *
 * NOTE: using library ArduinoJson 6.18.5
 */

#include <Wire.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>

#define WIFI_SSID "G41C control node"
#define WIFI_PASSWD "g41cstandsforgalc"
#define PORT 8080
#define ANALOG_IC_ADDR 0x36
#define DIGITAL_IC_ADDR 0x38
#define DIGITAL_IC_IN 0x00
#define DIGITAL_IC_OUT 0x01

/*const IPAddress local_IP(172, 16, 99, 100);
const IPAddress gateway(172, 16, 99, 1);
const IPAddress subnet(255, 255, 255, 0);*/

void configureDigitalIC();
void configureAnalogIC();

unsigned int readDigitalInputs();
void setDigitalOutput(const int pin, const bool state);

void readAnalogInput();
void handleConnections();
void connectWifi();

Servo doorServo;

bool buttonOutside = false;
bool buttonInside = false;
bool ledInside = false;
bool ledOutside = false;
int door = 0;
byte digitalBuffer = 0;

ArduinoWiFiServer server(PORT);

void setup()
{
    // Start up Serial debug
    Serial.begin(9600);

    connectWifi();

    Wire.begin();

    delay(1000);

    // Use pin d5 as servo output
    doorServo.attach(D5);
    pinMode(D5, OUTPUT);

    configureDigitalIC();

    // Start TCP Server
    server.begin();
    Serial.println("TCP Server started listening...");

    Serial.end();
}

void loop()
{
    // Handle clients sending request to the TCP server.
    handleConnections();

    int inputData = readDigitalInputs();

    buttonOutside = inputData & (1 << 0);
    buttonInside = inputData & (1 << 1);

    setDigitalOutput((ledInside << 5) | (ledOutside << 4));

    doorServo.write(door);
}

void handleConnections()
{
    // Gets a client that is connected to the server and has data available for reading.
    WiFiClient client = server.available();

    if (client) // Check if client has send a message, otherwise this is false.
    {
        String s = client.readStringUntil('}'); // Read the incoming message. Delimited by a new line char.

        StaticJsonDocument<100> jsonIn;
        deserializeJson(jsonIn, s);
        door = jsonIn["door"];
        ledInside = jsonIn["ledI"];
        ledOutside = jsonIn["ledO"];

        StaticJsonDocument<100> jsonOut;
        jsonOut["btnO"] = buttonOutside;
        jsonOut["btnI"] = buttonInside;

        String output;
        serializeJson(jsonOut, output);
        client.print(output);
    }
}

/* Read PCA9554 inputs (DIO0-DIO3) */
unsigned int readDigitalInputs()
{
    Wire.beginTransmission(DIGITAL_IC_ADDR); // Choose the PCA9554A
    Wire.write(byte(DIGITAL_IC_IN));         // Hex adress 0x00 to read inputs (DIO0-DIO3)
    Wire.endTransmission();                  // End I2C connection
    Wire.requestFrom(DIGITAL_IC_ADDR, 1);    // Request values from PCA9554A , 1 Byte

    unsigned int inputs = Wire.read(); // Copy values to variable inputs
    return (inputs & 0x0f);
}

/* Set PCA9554 outputs (DIO4-DIO7) */
void setDigitalOutput(byte digitalBuffer)
{
    Wire.beginTransmission(DIGITAL_IC_ADDR); // Choose the PCA9554A
    Wire.write(byte(DIGITAL_IC_OUT));        // Hex adress 0x01 to set outputs (DIO4-DIO7) to 1 or 0
    Wire.write(digitalBuffer);               // Set one of the output (DIO4-DIO7) to 1 or 0
    Wire.endTransmission();                  // End I2C connection
}

/* Configure PCA9554, Set DIO0-DIO3 as input & DIO4-DIO7 as output. */
void configureDigitalIC()
{
    Wire.beginTransmission(DIGITAL_IC_ADDR); // Choose the PCA9554A
    Wire.write(byte(0x03));                  // DDR register Hex address 0X03 to choose  input or output
    Wire.write(byte(0x0F));                  // 00001111   DIO0-DIO3 = INPUTS    DIO4-DIO7 = OUTPUTS
    Wire.endTransmission();                  // End I2C connection
}

/* Config MAX11647 Analog inputs */
void configureAnalogIC()
{
    Wire.beginTransmission(ANALOG_IC_ADDR); // Choose the MAX11647
    Wire.write(byte(0xA2));                 // set-up byte
    Wire.write(byte(0x03));                 // configuration byte
    Wire.endTransmission();                 // End I2C connection
}

/* Read the analog channel of the MAX11647 */
unsigned int readAnalogInput(int ANALOG_CH)
{
    unsigned int anin0;
    unsigned int anin1;

    // Read MAX11647
    if (ANALOG_CH == 0)
    {
        Wire.requestFrom(ANALOG_IC_ADDR, 4); // Request values from MAX11647 , 4 Bytes
        anin0 = Wire.read() & 0x03;          // AND values with 0000 0011 Copy values to variable anin0
        anin0 = anin0 << 8;                  // Shift anin0 8 places
        anin0 = anin0 | Wire.read();         // OR anin1 with data from analog ic
        return anin0;                        // Return value of anin0
    }

    if (ANALOG_CH == 1)
    {
        Wire.requestFrom(ANALOG_IC_ADDR, 4); // Request values from MAX11647 , 4 Bytes
        anin1 = Wire.read() & 0x03;          // AND values with 0000 0011 Copy values to variable anin1
        anin1 = anin1 << 8;                  // Shift anin1 8 places
        anin1 = anin1 | Wire.read();         // OR anin1 with data from analog ic
        return anin1;                        // Return value of anin1
    }
}

void connectWifi()
{
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    // Configure static IP.
    /*if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }*/

    // Start connecting to the WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);

    // Wait until we are connected
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Get IP Address of this device
    const IPAddress ip = WiFi.localIP();

    Serial.println();
    Serial.print("Connected to WiFi network: IP: ");
    Serial.println(ip);
}
