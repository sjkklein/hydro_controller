/*
* This sketch is written for the ESP8266 module.
* It uses the AP mode of the wifi module to broadcast a webpage.
* URL: 10.0.0.1 because mDNS won't work in AP mode
* The webpage contains sensor data that is collected via serial input.
* This is to aid me in monitoring plant conditions in my hydroponics system.
* 
* Thanks to Pieter P @ tttapa.github.io for the tutorials. They are excellent
*/

#include <ctype.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "home_page.h"
 
const char *ssid = "the_tower"; // The name of the Wi-Fi network that will be created
extern const char* root_page;
static float ph,water_temp,air_temp,humidity;
static int minute,second,hour;


ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handle_root();              // function prototypes for HTTP handlers
void handleNotFound();

void collect_data();
int verify_time(String,String,String);

void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to talk with arduino
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

  server.on("/", HTTP_GET, handle_root);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/set_time", HTTP_POST, set_time); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                       // Listen for HTTP requests from clients
  //handle_serial();
}

//converts variables to strings, puts it all in a buffer, then sends the homepage
void handle_root() { 
  char buff[2048],ph_buff[15],water_temp_buff[15],air_temp_buff[15],humidity_buff[15];
  
  collect_data(); //getting the time and sensor data from the arduino
  
  dtostrf(ph,4,2,ph_buff);
  dtostrf(water_temp,4,2,water_temp_buff);
  dtostrf(air_temp,4,2,air_temp_buff);
  dtostrf(humidity,4,2,humidity_buff);  
  sprintf(buff, root_page, hour, minute, second,
          ph_buff, air_temp_buff, water_temp_buff,
          humidity_buff,"full", "full");
  server.send(200, "text/html", buff);
}

//function called by HTTP POST on set time button on homepage
void set_time(){
  if ( ! server.hasArg("hour") || ! server.hasArg("minute")
       || ! server.hasArg("second") || server.arg("hour") == NULL
       || server.arg("minute") == NULL || server.arg("second") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  
  if(verify_time(server.arg("hour"),server.arg("minute"),server.arg("second"))){
    Serial.println("c "+server.arg("hour") + " " + server.arg("minute") + " " + server.arg("second"));
  }else{                                                                            // Username and password don't match
    server.send(401, "text/plain", "error: time not valid");
 }
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

int verify_time(String h,String m, String s){
  for(int i = 0; i< h.length(); i++)
    if(!isDigit(h.charAt(i)))
      return 0;

  for(int i = 0; i< m.length(); i++)
    if(!isDigit(m.charAt(i)))
      return 0;
      
  for(int i = 0; i< s.length(); i++)
    if(!isDigit(s.charAt(i)))
      return 0;

  if(h.toInt() > 23 || h.toInt() < 0)
    return 0;
    
  if(m.toInt() > 59 || m.toInt() < 0)
    return 0;

  if(s.toInt() > 59 || s.toInt() < 0)
    return 0;

  return 1;
}

void collect_data(){
  String time_str;
  String sensor_str;
  int count=0;
  Serial.println("t");
  while(!Serial.available() && count < 10000){
    delay(10);
    count++;
  }
  count = 0;
  if(Serial.read() == 't'){
    hour = Serial.parseInt();  
    minute = Serial.parseInt(); 
    second = Serial.parseInt();
  }else{
    hour=-1;
    minute=-1;
    second=-1;
  }
  Serial.flush();
  Serial.println("d");
  while(!Serial.available() && count < 100000){
    delay(10);
    count++;
  }
  if(Serial.read() == 'd'){
    humidity = Serial.parseFloat();
    air_temp = Serial.parseFloat();
    water_temp = Serial.parseFloat();
    ph = Serial.parseFloat();
  }
  Serial.flush();
}


