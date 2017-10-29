#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "home_page.h"
 
const char *ssid = "the_tower"; // The name of the Wi-Fi network that will be created
extern const char* root_page;
static float ph,water_temp,air_temp,humidity;
static int minute,second,hour;


ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleLogin();
void handleNotFound();

void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  IPAddress apLocalIp(10, 0, 0, 1);
  IPAddress gateway(192, 168, 4, 9);
  IPAddress apSubnetMask(255, 255, 255, 0);
  Serial.println(WiFi.softAPConfig(apLocalIp, gateway, apSubnetMask ) ? "Ready" : "Failed!");
  WiFi.softAP(ssid);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);

  Serial.println("\" started");
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer

  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/set_time", HTTP_POST, set_time); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                     // Listen for HTTP requests from clients
}

void handleRoot() {  // When URI / is requested, send a web page with a button to toggle the LED
  char buff[2048],ph_buff[15],water_temp_buff[15],air_temp_buff[15],humidity_buff[15];
  dtostrf(ph,4,2,ph_buff);
  dtostrf(water_temp,4,2,water_temp_buff);
  dtostrf(air_temp,4,2,air_temp_buff);
  dtostrf(humidity,4,2,humidity_buff);  
  sprintf(buff, root_page, hour, minute, second,
          ph_buff, air_temp_buff, water_temp_buff,
          humidity_buff,"full", "full");
  server.send(200, "text/html", buff);
}

void set_time(){
  if ( ! server.hasArg("hour") || ! server.hasArg("minute")
       || ! server.hasArg("second") || server.arg("hour") == NULL
       || server.arg("minute") == NULL || server.arg("second") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  Serial.println(server.arg("hour")+":"+server.arg("minute")+":"+server.arg("second"));
  
  if(verify_time())
  
  }else{                                                                            // Username and password don't match
    server.send(401, "text/plain", "error: time not valid");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}



