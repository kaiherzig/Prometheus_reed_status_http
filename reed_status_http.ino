#include <ESP8266WiFi.h>

String header;
const int statusPin = 13;

WiFiServer server(80);
WiFiClient client;

const char* SSID = "<<FILLME>>";
const char* PASS = "<<FILLME>>";

void setup() {
  Serial.begin(115200);
  pinMode(statusPin, INPUT_PULLUP);
  delay(50);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  delay(1000);

  //Start the server
  server.begin();
  Serial.println("Server started");

  //Print the IP Address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void loop() 
{
  WiFiClient client = server.available();
  //CheckDoorStatus();
  if (client) {
    Serial.println("Neuer Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            int newStatus = digitalRead(statusPin);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println(newStatus);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client getrennt.");
    Serial.println("");
  }

}
