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
 *       using Adafruit_NeoPixel 1.10.1
 */

#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>
#include <Adafruit_NeoPixel.h>

#define WIFI_SSID "3.1415"
#define WIFI_PASSWD "YouShallNotPassword"
#define PORT 8080
#define DIGITAL_IC_ADDR 0x38
#define DIGITAL_IC_IN 0x00
#define DIGITAL_IC_OUT 0x01

#define NUM_LEDS 1
#define DATA_PIN D5

const IPAddress local_IP(172, 16, 99, 101);
const IPAddress gateway(172, 16, 99, 1);
const IPAddress subnet(255, 255, 255, 0);

void configureDigitalIC();

unsigned int readDigitalInputs();
void setDigitalOutput(const int pin, const bool state);

void handleConnections(WiFiClient client);
void connectWifi();

bool pirSensor = false;
uint32_t rgb0 = 0;
uint32_t rgb0history = 0;

ArduinoWiFiServer server(PORT);
Adafruit_NeoPixel pixels(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();

  // Start up Serial debug
  Serial.begin(9600);

  connectWifi();

  Wire.begin();

  delay(1000);

  configureDigitalIC();

  // Start TCP Server
  server.begin();
  Serial.println("TCP Server started listening...");
}

void loop()
{
  WiFiClient client = server.available();

  // Don't check for an available client while we still know an connected client.
  while (client.connected())
  {
    int inputData = readDigitalInputs();
    pirSensor |= inputData == 15;

    // Check if client has send a message, otherwise this is false.
    if (client)
    {
      // Handle clients sending request to the TCP server.
      handleConnections(client);

      if (rgb0history != rgb0)
      {
        pixels.setPixelColor(0, rgb0);
        pixels.show();
        rgb0history = rgb0;
      }
    }
  }
}

void handleConnections(WiFiClient client)
{
  String s = client.readStringUntil('}'); // Read the incoming message. Delimited by a new line char.

  StaticJsonDocument<100> jsonIn;
  deserializeJson(jsonIn, s);
  rgb0 = jsonIn["rgb0"];

  StaticJsonDocument<100> jsonOut;
  jsonOut["pir"] = pirSensor;

  String output;
  serializeJson(jsonOut, output);
  client.print(output);

  pirSensor = false;
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
void setDigitalOutput(int pin, bool state)
{
  Wire.beginTransmission(DIGITAL_IC_ADDR); // Choose the PCA9554A
  Wire.write(byte(DIGITAL_IC_OUT));        // Hex adress 0x01 to set outputs (DIO4-DIO7) to 1 or 0
  Wire.write(byte(state << pin));          // Set one of the output (DIO4-DIO7) to 1 or 0
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
