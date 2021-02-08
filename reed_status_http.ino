/*
 * UNTESTED!!!!!
 */

#include <ESP8266WiFi.h>

#define WIFI_SSID "<<FILLME>>"
#define WIFI_KEY "<<FILLME>>"

// Pin to reed, other to GND
uint8_t inPin = D5;

WiFiServer server(80);

void setup() {
  pinMode(inPin, INPUT);
  Serial.begin(115200);

  // Initiate Wi-Fi connection setup
  WiFi.begin(WIFI_SSID, WIFI_KEY);

  // Show status on serial monitor
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}




// This is the main section and will loop continuously
void loop() {

  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New client");
    boolean blank_line = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

        if (c == '\n' && blank_line)
        {
          String door_state;

          if (digitalRead(inPin) == LOW)
          {
            door_state = "false";
          }
          else
          {
            door_state = "true";
          }

          Serial.println(door_state);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head>");
          client.println("<body>");
          client.println(door_state);
          client.println("</body></html>");
          break;
        }
        if (c == '\n')
        {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r')
        {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }

}
