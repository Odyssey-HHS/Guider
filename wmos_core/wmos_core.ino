/*
 * Wemos I/O Server
 *
 * Wemos module connecting to a wifi network and creating a TCP server.
 *
 * Can receive TCP server commands to set or get data.
 *
 * NOTE: using library ArduinoJson 6.18.5
 */
#include <Wire.h>
#include <Servo.h>
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


void Config_Digital_IC();
void set_ouput(int pin, bool state);
unsigned int read_inputs();
void read_analog_inputs();
void Config_Analog_IC();
void handleConnections();





Servo myservo;

//   0x36  =  i2c address MAX11647
//   0x38  =  i2c address PCA9554A
int ADDRESS0 = 0x36;  //Analog inputs IC
int ADDRESS1 = 0x38;  //DIO0-DIO7 IC

unsigned int output_adress = 0x01;      // OPR ADRESS
unsigned int input_adress = 0X00;       // IPR ADRESS

unsigned int input_data = 0;

bool Knop_buiten;
bool Knop_binnen;
bool ledIn;
bool ledUit;
int deur = 0;


ArduinoWiFiServer server(8080);

void setup()
{
    // Start up Serial debug
    Serial.begin(9600);

    Wire.begin();

    delay(1000);

    myservo.attach(D5);
    pinMode(D5, OUTPUT);

    Config_Digital_IC();



    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Configure static IP.
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }

    // Start connecting to the WiFi
    WiFi.begin(ssid, password);

    // Wait until we are connected
    while (WiFi.status() != WL_CONNECTED)
    {
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


    Serial.end();
}



void loop()
{
    // Handle clients sending request to the TCP server.
    handleConnections();

    input_data = read_inputs();


    if (input_data == 13)
    {
        Knop_buiten = true;
    }

    else
    {
        Knop_buiten = false;

    }


    if (input_data == 14)
    {
        Knop_binnen = true;
    }
    else
    {

        Knop_binnen = false;
    }

    set_ouput(4, ledIn);
    set_ouput(5, ledUit);

    myservo.write(deur);
}

void handleConnections()
{
    // Gets a client that is connected to the server and has data available for reading.
    WiFiClient client = server.available();

    if (client)   // Check if client has send a message, otherwise this is false.
    {
        String s = client.readStringUntil('}'); // Read the incoming message. Delimited by a new line char.

        StaticJsonDocument<70> jsonIn;
        deserializeJson(jsonIn, s);
        deur = jsonIn["door"];
        ledIn = jsonIn["ledIn"];
        ledUit = jsonIn["ledOut"];

        StaticJsonDocument<70> jsonOut;
        jsonOut["buttonOut"] = Knop_buiten;
        jsonOut["buttonIn"] = Knop_binnen;

        String output;
        serializeJson(jsonOut, output);
        client.print(output);
    }
}




unsigned int read_inputs()
{


    //Read PCA9554 inputs (DIO0-DIO3)

    unsigned int inputs = 0;

    Wire.beginTransmission(ADDRESS1);        // Choose the PCA9554A
    Wire.write(byte(input_adress));         //  Hex adress 0x00 to read inputs (DIO0-DIO3)
    Wire.endTransmission();                 // End I2C connection

    Wire.requestFrom(ADDRESS1, 1);         // Request values from PCA9554A , 1 Byte
    inputs = Wire.read();                  // Copy values to variable inputs

    return (inputs & 0x0f);

}



void set_ouput(int pin, bool state)
{

//Set PCA9554 outputs (DIO4-DIO7)
    Wire.beginTransmission(ADDRESS1);       //Choose the PCA9554A
    Wire.write(byte(output_adress));        //  Hex adress 0x01 to set outputs (DIO4-DIO7) to 1 or 0
    Wire.write(byte(state << pin));               // set the output DIO4 to 1
    Wire.endTransmission();                  // End I2C connection



}


void Config_Digital_IC()
{


    //Config PCA9554
    //Set DIO0-DIO3 as input & DIO4-DIO7 as output.
    Wire.beginTransmission(ADDRESS1);  //Choose the PCA9554A
    Wire.write(byte(0x03));          // DDR register Hex address 0X03 to choose  input or output
    Wire.write(byte(0x0F));          // 00001111   DIO0-DIO3 = INPUTS    DIO4-DIO7 = OUTPUTS
    Wire.endTransmission();         // End I2C connection


}




void Config_Analog_IC()
{

//Config MAX11647 Analog inputs
    Wire.beginTransmission(ADDRESS0);     //Choose the MAX11647
    Wire.write(byte(0xA2));              //set-up byte
    Wire.write(byte(0x03));              //configuration byte
    Wire.endTransmission();             // End I2C connection


}



void read_analog_inputs()
{

    unsigned int anin0;
    unsigned int anin1;

    // Read MAX11647
    Wire.requestFrom(ADDRESS0, 4);   // Request values from MAX11647 , 4 ByteS
    anin0 = Wire.read() & 0x03;     //   // AND values with 0000 0011   Copy values to variable anin0
    anin0 = anin0 << 8;
    anin0 = anin0 | Wire.read();
    anin1 = Wire.read() & 0x03;
    anin1 = anin1 << 8;
    anin1 = anin1 | Wire.read();

}
