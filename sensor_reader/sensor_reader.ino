/*
 * This code is written to measure some sensor data for my hydroponics system.
 * 
 * NOT FULLY IMPLEMENTED
 * I have pumps to control ph but i have not tested controlling it automatically yet.
 * 
 */

#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//these macros are here to preset the time to when the device is programmed.
#define SECOND 40
#define MINUTE 31
#define HOUR   11

#define LED 13
#define DHT_PIN 9
#define DB18_PIN 4
#define PH_PIN A0
#define PH_PWR 15
#define PH_GND 16
#define RES_SWITCH 3 
#define BAK_SWITCH 2 
#define LIGHT_PIN 10
#define MAIN_PUMP 11

#define SECOND_PUMP 8
#define PH_UP 6
#define PH_DOWN 5


#define PH_OFFSET 0.02            //deviation compensate


#define DHT_TYPE DHT11   // DHT 11 



/*
 accepted commands
 's' int int int --sets the real time clock
 't' --arduino returns time in format: 't' int int int
 'd' --arduino sends sensor data
 
*/

/*
int ledPin=13;
int pd2=2;
int pd3=3;
int pd4=4;
int pd5=5;
int pd6=6;
int a6=19;
*/
int now=0;
int way_back_when=0;
int millisecond=0,minute=MINUTE,second=SECOND,hour=HOUR;

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DB18_PIN);
DallasTemperature db18(&oneWire);

char str[200];

void toggle_led(){
  static int blan;
  if(blan%2){
    digitalWrite(LED,LOW);
  }else{
    digitalWrite(LED,HIGH);
  }
  blan++;
}

void setup() {
  Serial.setTimeout(100);
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(PH_PWR, OUTPUT);
  pinMode(PH_GND, OUTPUT);

  pinMode(RES_SWITCH, INPUT);
  pinMode(BAK_SWITCH, INPUT);

  pinMode(MAIN_PUMP, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(SECOND_PUMP, OUTPUT);

  pinMode(PH_UP, OUTPUT);
  pinMode(PH_DOWN, OUTPUT);  

  
  digitalWrite(PH_PWR,1);
  digitalWrite(PH_GND,0);
  digitalWrite(PH_UP,0);
  digitalWrite(PH_DOWN,0);
  digitalWrite(SECOND_PUMP,0);
  digitalWrite(MAIN_PUMP,1);
  digitalWrite(LIGHT_PIN,1);//1 = off


  
  dht.begin();
  db18.begin();
  
}

void loop() {
  parse_instr();
  now=millis();
  if(now!=way_back_when){
    millisecond+=now-way_back_when;
    way_back_when=now;
    if(millisecond>=999){
      second++;
      toggle_led();
      digitalWrite(SECOND_PUMP, digitalRead(RES_SWITCH));
      millisecond=0;
      if(second>59){
        minute++;
        second=0;
        if(minute>59){
          hour++;
          minute=0;
          if(hour>23){
            hour=0;
          }          
        }
        alarms();
      }
    }
  }
}

void parse_instr(){
  char instr;
  int s,m,h;
  if(Serial.available()){
    instr = Serial.read();
    if(instr == 's'){
      //setting the clock to the new time
      h=Serial.parseInt();
      m=Serial.parseInt();
      s=Serial.parseInt(); 
      if(1/*validate_time(s,m,h)*/){
        minute=m;
        second=s;
        hour=h;   
      }
    }else if(instr ==  'd'){
      send_sensor_data();
    }else if(instr == 't'){
      sprintf(str," t %d %d %d\n",hour,minute,second);
      Serial.print(str);
    }
    else{
      Serial.flush();
    }
  }else{
  }
  
}

float measure_ph(){
  long raw=0;
  float voltage;
  for(int i=0;i<100;i++)
    raw += analogRead(PH_PIN);
  voltage = (raw/100)*5.0/1024;
  return 3.5*voltage + PH_OFFSET;
}

void send_sensor_data(){
db18.requestTemperatures();

Serial.print(" d ");//humidity
Serial.print( dht.readHumidity() );
Serial.print(" ");//airtemp
Serial.print( dht.readTemperature(true) );
Serial.print(" ");//watertemp
Serial.print( db18.getTempFByIndex(0) );
Serial.print(" ");//ph
Serial.print(measure_ph()); 
Serial.print(" ");//resevoir level
Serial.print(digitalRead(RES_SWITCH));
Serial.print(" ");//backup water level
Serial.print(digitalRead(BAK_SWITCH));
Serial.print("\n");
}

void alarms(){
//  Serial.println(minute%15);
  if(minute%15 == 0){
    digitalWrite(MAIN_PUMP, !digitalRead(MAIN_PUMP));
  }
  if(hour<6){
    digitalWrite(LIGHT_PIN,1);
  }else{
    digitalWrite(LIGHT_PIN, 0);
  }
}


