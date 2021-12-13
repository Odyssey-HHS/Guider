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

#include <Servo.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>

#include "DigitalIO.h"
#include "AnalogI.h"

#define WIFI_SSID "3.1415"
#define WIFI_PASSWD "YouShallNotPassword"
#define PORT 8080
#define ANALOG_IC_ADDR 0x36
#define DIGITAL_IC_ADDR 0x38

const IPAddress local_IP(172, 16, 99, 100);
const IPAddress gateway(172, 16, 99, 1);
const IPAddress subnet(255, 255, 255, 0);

void handleConnections();
void connectWifi();

Servo doorServo;

bool buttonOutside = false;
bool buttonInside = false;
bool ledInside = false;
bool ledOutside = false;
int door = 0;

ArduinoWiFiServer server(PORT);
DigitalIO digitalChip(DIGITAL_IC_ADDR);
AnalogI analogChip(ANALOG_IC_ADDR);

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

    // Start TCP Server
    server.begin();
    Serial.println("TCP Server started listening...");

    Serial.end();
}

void loop()
{
    // Handle clients sending request to the TCP server.
    handleConnections();

    int inputData = digitalChip.readInput();

    buttonOutside = inputData == 13;
    buttonInside = inputData == 14;

    digitalChip.setOutput(4, ledInside);
    digitalChip.setOutput(5, ledOutside);

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



void connectWifi()
{
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    // Configure static IP.
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }

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
