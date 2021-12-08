/*
 * Wemos I/O Server
 * 
 * Wemos module connecting to a wifi network and creating a TCP server.
 * 
 * Can receive TCP server commands to set or get data.
 * 
 * NOTE: using library ArduinoJson 6.18.5
 */
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>

#define STASSID "3.1415"
#define STAPSK  "YouShallNotPassword"

IPAddress local_IP(172, 16, 99, 100);
IPAddress gateway(172, 16, 99, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid     = STASSID;
const char* password = STAPSK;

bool led0 = false;
bool led1 = true;

ArduinoWiFiServer server(8080);

void setup() {
  // Start up Serial debug
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Configure static IP.
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // Start connecting to the WiFi
  WiFi.begin(ssid, password);

  // Wait until we are connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Get IP Address of this device
  IPAddress ip = WiFi.localIP();

  Serial.println();
  Serial.print("Connected to WiFi network: IP: ");  
  Serial.println(ip);

  // Start TCP Server
  server.begin();
  Serial.println("TCP Server started listening...");

  // Leds
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  delay(500);
  Serial.end();
}

void handleConnections();

void loop() {
  // Handle clients sending request to the TCP server.
  handleConnections();

  digitalWrite(5, led0);
  digitalWrite(16, led1);

}

void handleConnections() {
  // Gets a client that is connected to the server and has data available for reading.
  WiFiClient client = server.available();
  
  if (client) { // Check if client has send a message, otherwise this is false.
    String s = client.readStringUntil('}'); // Read the incoming message. Delimited by a new line char.

    StaticJsonDocument<70> jsonIn;
    deserializeJson(jsonIn, s);
    led0 = jsonIn["led0"];
    led1 = jsonIn["led1"];

    StaticJsonDocument<70> jsonOut;
    jsonOut["button0"] = !digitalRead(4);

    String output;
    serializeJson(jsonOut, output);
    client.print(output);
  }
}
