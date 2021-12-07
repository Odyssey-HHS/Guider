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

#ifndef STASSID
#define STASSID "G41C control node"
#define STAPSK  "g41cstandsforgalc"
#endif

//IPAddress local_IP(172, 16, 99, 101);
//IPAddress gateway(172, 16, 99, 1);
//IPAddress subnet(255, 255, 255, 0);

const char* ssid     = STASSID;
const char* password = STAPSK;

ArduinoWiFiServer server(8080);

void setup() {
  // Start up Serial debug
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Configure static IP.
//  if (!WiFi.config(local_IP, gateway, subnet)) {
//    Serial.println("STA Failed to configure");
//  }

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

  // Set the I/O

  // Buttons
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);

  // Leds
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

void handleConnections();

void loop() {
  // Handle clients sending request to the TCP server.
  handleConnections();
}

void handleConnections() {
  // Gets a client that is connected to the server and has data available for reading.
  WiFiClient client = server.available();
  
  if (client) { // Check if client has send a message, otherwise this is false.
    Serial.println("Got something");
    String s = client.readStringUntil(';'); // Read the incoming message. Delimited by a new line char.
    s.trim(); // trim eventual \r
    Serial.print("Incoming message from client; ");
    Serial.println(s); // print the message to Serial Monitor

    //deserialize incoming json string
    StaticJsonDocument<70> jsons;
    
    deserializeJson(jsons, s);
    Serial.println((String) jsons["var1"]);
    Serial.println((String) jsons["var2"]);


    //serialize a new json string with the vars swapped around
    StaticJsonDocument<70> jsonOut;
    jsonOut["var1"] = jsons["var2"];
    jsonOut["var2"] =  jsons["var1"];

    String output;
    serializeJson(jsonOut, output);

    client.print(output);
  }
}
