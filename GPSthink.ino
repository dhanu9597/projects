#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
float latitude , longitude;
String  lat_str , lng_str;
// repace your wifi username and password
const char* ssid     = "dhanu";
const char* password = "12345678";
unsigned long myChannelNumber = 991048;
const char * myWriteAPIKey = "R9J7MM0ENG0PO45P";
// The TinyGPS++ object
TinyGPSPlus gps;
WiFiClient  client;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);
}
void loop()
{
  while (ss.available() > 0){
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.print("Latitude = ");
        Serial.println(lat_str);
        Serial.print("Longitude = ");
        Serial.println(lng_str);
        ThingSpeak.setField(1, lat_str);
        ThingSpeak.setField(2, lng_str);
        ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
      }
     delay(1000);
     Serial.println();  
    }
  }
}
