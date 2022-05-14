#include <Arduino.h>
#include <WiFi.h>
#include <stdio.h>
#include "Pages/Swagger_page.h"
#include "Pages/Swagger_json.h"

extern const char * swaggerUI;
extern const char * swaggerJSON;

const char* ssid = "DESKTOP-M2JI90L 6706";
const char* password = "1R24r2&9";

WiFiServer server(80); // Port number = 80

String header;          // Variable to store the HTTP request

// Auxiliar variables to store the current output state
String PIN_2State = "off";
String output27State = "off";

const int PIN_2 = 2;    //Pin to controller
const int output27 = 27;

// Timers
unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0;       // Previous time
const long timeoutTime = 2000;        // Define timeout time in milliseconds (example: 2000ms = 2s)

/* Functions Predefinitions*/
void page_format(WiFiClient* client);
void swagger_page(WiFiClient* client);
void swagger_Json(WiFiClient* client);
void Axis_xy(WiFiClient* client);

void setup() {
  Serial.begin(9600);
  // Initialize output
  pinMode(PIN_2, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(PIN_2, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin(); //Start the server
}

void loop(){

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // End points
            if (header.indexOf("GET /Axis_xy") >= 0){
              Axis_xy(&client);
            }
            else if (header.indexOf("GET /docs") >= 0) {
              swagger_page(&client);
            }
            else if (header.indexOf("GET /swagger.json") >= 0) {
              swagger_Json(&client);
            }
            else if (header.indexOf("GET /LEDS") >= 0) {
              page_format(&client);
            }
            else if (header.indexOf("GET /26/on") >= 0) {
              PIN_2State = "on";
              digitalWrite(PIN_2, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              PIN_2State = "off";
              digitalWrite(PIN_2, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            break; // Break out of the while loop

          } else currentLine = ""; // if you got a newline, then clear currentLine
          
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}

void page_format(WiFiClient* client){

  // Display the HTML web page
  client->println("<!DOCTYPE html><html>");
  client->println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client->println("<link rel=\"icon\" href=\"data:,\">");

  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client->println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client->println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client->println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client->println(".button2 {background-color: #555555;}</style></head>");
  
  // Web Page Heading
  client->println("<body><h1>ESP32 Web Server</h1>");

  client->println("<p>GPIO 26 - State " + PIN_2State + "</p>"); // Display GPIO 26 
  // If the PIN_2State is off, it displays the ON button       
  if (PIN_2State=="off")  client->println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
  else                    client->println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");               
  
  client->println("<p>GPIO 27 - State " + output27State + "</p>"); // Display GPIO 27
  // If the output27State is off, it displays the ON button       
  if (output27State=="off") client->println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
  else                      client->println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");

  client->println("</body></html>");
  
  // The HTTP response ends with another blank line
  client->println();
}

void swagger_page(WiFiClient* client){
  client->println(swaggerUI);
}

void swagger_Json(WiFiClient* client){
  client->println(swaggerJSON);
}

void Axis_xy(WiFiClient* client){
  int AD_x = analogRead(33);
  int AD_y = analogRead(32);
  char answer [50];

  sprintf(answer, "[{\"x\":\"%d\",\"y\":\"%d\"}]", AD_x, AD_y);
  client->println(answer);
}