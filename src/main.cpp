#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define DHTPIN 2            // temperature and humidity
#define MQ2PIN 34           // gas
#define MOTION 13           // motion
#define LIGHT 35           // light
#define TRIG 16             // distance
#define ECHO 17             // distance
#define LED 19
#define GAS 18
#define FAN 5
#define SERVOO 27
#define VENTILATION 25
#define SCREEN_WIDTH 128
#define SCREEN_LENGTH 64
#define SCREEN_RESET -1

#define DHTTYPE DHT22
Servo servo;
Servo venti;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 pressure;

float temp;
float hum;
int smokesensor;
int mot;
float lit;
String mode="AUTO";
String WiFiStatus;
String MQTTStatus;


const float GAMMA = 0.7;
const float RL10 = 50;

float distance;
int32_t pre;
boolean PressureOK;

const char* ssid="Wokwi-GUEST";
const char* password="";
const char* mqtt_server="broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMessage=0;

//============================WIFI SETUP=========================

void wifi_setup()
{
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
  }
  if(WiFi.status()==WL_CONNECTED)
  {
    WiFiStatus="CONNECTED";
  }
  else
  {
    WiFiStatus="NOT CONNECTED";
  }
}


void reconnect()
{
  while(!client.connected())
  {
    String clientID="Wokwi-ESP32-";
    clientID=clientID+String(random(0xffff),HEX);

    if(client.connect(clientID.c_str()))
    {
      client.subscribe("Harshal_SmartHome/control");

    }
    else 
    {
      delay(5000);
    }
  }
}

// ==================== STATUS VARIABLES ====================

String temperatureStatus;
String humidityStatus;
String gasStatus;
String motionStatus;
String lightStatus;
String distanceStatus;
String pressureStatus;

String roomLightStatus="OFF";
String buzzerStatus="OFF";
String fanStatus="OFF";
String doorStatus="CLOSED";
String ventilationStatus="CLOSED";


//=======================DISPLAY============================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_LENGTH, &Wire, SCREEN_RESET); 

//=====================MQTT MESSAGE=====================
void callback(char* topic, byte* payload, unsigned int length)
{
  String message;
  Serial.println("===== MQTT MESSAGE RECEIVED =====");
Serial.print("Topic   : ");
Serial.println(topic);
Serial.print("Payload : ");
Serial.println(message);
Serial.println("===============================");
  for(unsigned int i=0; i<length;i++)
  {
    message = message + (char)payload[i];

  }
    if(message=="MANUAL")
{
    mode="MANUAL";
}
else if(message=="AUTO")
{
    mode="AUTO";
}
else if(mode=="MANUAL")
{
    if(message=="LIGHT_ON")
    {
        roomLightStatus="ON";
        digitalWrite(LED,HIGH);
    }

    else if(message=="LIGHT_OFF")
    {
        roomLightStatus="OFF";
        digitalWrite(LED,LOW);
    }

    else if(message=="FAN_ON")
    {
        fanStatus="ON";
        digitalWrite(FAN,HIGH);
    }

    else if(message=="FAN_OFF")
    {
        fanStatus="OFF";
        digitalWrite(FAN,LOW);
    }

    else if(message=="DOOR_OPEN")
    {
        doorStatus="OPEN";
        servo.write(90);
    }

    else if(message=="DOOR_CLOSE")
    {
        doorStatus="CLOSED";
        servo.write(0);
    }

    else if(message=="VENT_OPEN")
    {
        ventilationStatus="OPEN";
        venti.write(90);
    }

    else if(message=="VENT_CLOSE")
    {
        ventilationStatus="CLOSED";
        venti.write(0);
    }

    else
    {
        Serial.print("Unknown MQTT Command: ");
        Serial.println(message);
    }
}


}

void CheckWIFI()
{
  if(WiFi.status()!=WL_CONNECTED)
  { 
    WiFiStatus="NOT CONNECTED";
    Serial.println("WiFi Lost!");
    Serial.println("Reconnecting...");

    WiFi.disconnect();
    WiFi.begin(ssid, password);
    while(WiFi.status()!=WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi Reconnected!");

        WiFiStatus = "CONNECTED";
    }
    else
    {
        WiFiStatus = "CONNECTED";
    }


  }




void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(micros());
  //display initialisation
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("Screen allocation failed");
    for(;;);
  }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(8,8);
    display.println("SMART");
    display.println(" HOME");
    display.display();
    delay(1500);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);

    display.println("SYSTEM STARTUP");
    display.println("--------------------");
    display.println("[OK] OLED");
    display.println("[OK] Sensors");
    display.println();
    display.println("Initializing...");
    display.display();
    delay(1000);
  //wifi setup

  display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);

    display.println("NETWORK");
    display.println("--------------------");
    display.println("Connecting WiFi...");
    display.display();
    delay(1000);

  wifi_setup();

  display.clearDisplay();
  display.setCursor(0,0);

  display.println("NETWORK");
  display.println("--------------------");
  display.println("WiFi Connected");
  display.println();
  display.println(WiFi.localIP());
  display.display();
  delay(1000);

  //mqtt setup
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
  reconnect();
    display.clearDisplay();
    display.setCursor(0,0);

    display.println("NETWORK");
    display.println("--------------------");
    display.println("MQTT Connected");
    display.println();
    display.println("Broker Ready");
    display.display();
    delay(1000);

  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(GAS,OUTPUT);
  pinMode(FAN,OUTPUT);
  digitalWrite(FAN,LOW);
  servo.attach(SERVOO);
  venti.attach(VENTILATION);
  servo.write(0);
  venti.write(0);

  Serial.println();
  Serial.println("========================================");
  Serial.println("       SMART HOME SYSTEM STARTUP");
  Serial.println("========================================");

  // ==================== TEMPERATURE ====================

  dht.begin();
  Serial.println("[OK] Temperature sensor initiated");

  // ==================== HUMIDITY ====================

  Serial.println("[OK] Humidity sensor initiated");

  // ==================== GAS ====================

  Serial.println("[OK] Gas sensor initiated");

  // ==================== MOTION ====================

  Serial.println("[OK] Motion sensor initiated");

  // ==================== LIGHT ====================

  Serial.println("[OK] Light sensor initiated");

  // ==================== DISTANCE ====================

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("[OK] Distance sensor initiated");

  // ==================== PRESSURE ====================

  Wire.begin(21, 22);

  PressureOK = pressure.begin();

  if (PressureOK)
  {
    Serial.println("[OK] Pressure sensor initiated");
  }
  else
  {
    Serial.println("[ERROR] Pressure sensor failed");
  }

  Serial.println("========================================");
  Serial.println("          SYSTEM READY");
  Serial.println("========================================");


  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(8,0);
  display.println("READY");

  display.setTextSize(1);
  display.println();
  display.println("--------------------");
  display.println("Smart Home Online");
  display.println("Mode : AUTO");

  display.display();
  delay(2000);
}


void loop() {

  //-------------------WIFI----------
  CheckWIFI();
  //------------------MQTT----------------------
  if(!client.connected())
  {
      reconnect();
  }

  MQTTStatus = client.connected() ? "CONNECTED" : "NOT CONNECTED";
  
  client.loop();
  //--------------------------------------------------------
  Serial.println();
  Serial.println("========================================");
  Serial.println("         SMART HOME LIVE DATA");
  Serial.println("========================================");


  //----------------------TEMPERATURE-------------------------

  temp = dht.readTemperature();

  if (isnan(temp))
  {
    temperatureStatus = "ERROR";

    Serial.println("TEMPERATURE : ERROR");
  }
  else
  {
    if (temp <= 18)
    {
      temperatureStatus = "COLD";
    }
    else if (temp < 28 && temp > 18)
    {
      temperatureStatus = "COMFORTABLE";
    }
    else
    {
      temperatureStatus = "HOT";
    }

    Serial.println(
      "TEMPERATURE : " + String(temp) + " C"
      + "  |  STATUS: " + temperatureStatus
    );
  }


  //----------------------HUMIDITY----------------------------

  hum = dht.readHumidity();

  if (isnan(hum))
  {
    humidityStatus = "ERROR";

    Serial.println("HUMIDITY    : ERROR");
  }
  else
  {
    if (hum <= 30)
    {
      humidityStatus = "DRY";
    }
    else if (hum > 30 && hum < 60)
    {
      humidityStatus = "MODERATE";
    }
    else
    {
      humidityStatus = "HUMID";
    }

    Serial.println(
      "HUMIDITY    : " + String(hum) + " %"
      + "  |  STATUS: " + humidityStatus
    );
  }


  //----------------------AIR-------------------------------

  smokesensor = analogRead(MQ2PIN);

  if (smokesensor <= 3578)
  {
    gasStatus = "SAFE";
  }
  else if (smokesensor > 3578 && smokesensor <= 3713)
  {
    gasStatus = "WARNING";
  }
  else
  {
    gasStatus = "DANGER";
  }

  Serial.println(
    "GAS LEVEL   : " + String(smokesensor) + " ADC"
    + "  |  STATUS: " + gasStatus
  );


  //---------------------MOTION-----------------------------

  mot = digitalRead(MOTION);

  if (mot == HIGH)
  {
    motionStatus = "DETECTED";
  }
  else
  {
    motionStatus = "NONE";
  }

  Serial.println(
    "MOTION      : " + motionStatus
  );


  //---------------------LIGHT---------------------------------

  int analogValue = analogRead(LIGHT);

  float voltage = analogValue / 4096. * 5;

  float resistance = 2000 * voltage / (1 - voltage / 5);

  lit = pow(
    RL10 * 1e3 * pow(10, GAMMA) / resistance,
    (1 / GAMMA)
  );

  if (lit <= 50)
  {
    lightStatus = "DARK";
  }
  else if (lit > 50 && lit <= 500)
  {
    lightStatus = "MODERATE";
  }
  else
  {
    lightStatus = "BRIGHT";
  }

  Serial.println(
    "LIGHT       : " + String(lit) + " lux"
    + "  |  STATUS: " + lightStatus
  );


  //---------------------DISTANCE-----------------------------

  digitalWrite(TRIG, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  int duration = pulseIn(ECHO, HIGH, 30000);

  if (duration == 0)
  {
    distanceStatus = "NO OBJECT";

    Serial.println(
      "DISTANCE    : NO OBJECT / OUT OF RANGE"
    );
  }
  else
  {
    distance = duration / 58.0;

    if (distance <= 30)
    {
      distanceStatus = "VERY NEAR";
    }
    else if (distance > 30 && distance <= 100)
    {
      distanceStatus = "NEAR";
    }
    else
    {
      distanceStatus = "FAR";
    }

    Serial.println(
      "DISTANCE    : " + String(distance) + " cm"
      + "  |  STATUS: " + distanceStatus
    );
  }


  //---------------------PRESSURE--------------------------------

  if (PressureOK)
  {
    pre = pressure.readPressure();

    if (pre <= 100000)
    {
      pressureStatus = "LOW";
    }
    else if (pre > 100000 && pre <= 102000)
    {
      pressureStatus = "NORMAL";
    }
    else
    {
      pressureStatus = "HIGH";
    }

    Serial.println(
      "PRESSURE    : " + String(pre) + " Pa"
      + "  |  STATUS: " + pressureStatus
    );
  }
  else
  {
    pressureStatus = "ERROR";

    Serial.println(
      "PRESSURE    : ERROR"
    );
  }


  //---------------------MISCELLANEOUS--------------

  Serial.println("----------------------------------------");
  Serial.println("         NEXT UPDATE IN 10 SECONDS");
  Serial.println("========================================");

  







  
//====================================================
//                 ACTUATORS
//====================================================

Serial.println();
Serial.println("========================================");
Serial.println("           ACTUATOR STATUS");
Serial.println("========================================");


if(mode=="AUTO")
{
      // ROOM LED
    if(lightStatus=="DARK" && motionStatus=="DETECTED")
    {
        digitalWrite(LED,HIGH);
        roomLightStatus = "ON";
    }
    else
    {
        digitalWrite(LED,LOW);
        roomLightStatus = "OFF";
    }


    // GAS BUZZER
    if(gasStatus == "DANGER")
    {
        digitalWrite(GAS,HIGH);
        buzzerStatus = "ON";
    }
    else
    {
        digitalWrite(GAS,LOW);
        buzzerStatus = "OFF";
    }


    // FAN
    if(temperatureStatus=="HOT")
    {
        digitalWrite(FAN,HIGH);
        fanStatus = "ON";
    }
    else
    {
        digitalWrite(FAN,LOW);
        fanStatus = "OFF";
    }


    // DOOR SERVO
    if(distanceStatus=="VERY NEAR")
    {
        servo.write(90);
        doorStatus = "OPEN";
    }
    else
    {
        servo.write(0);
        doorStatus = "CLOSED";
    }


    // VENTILATION SERVO
    if(temperatureStatus=="HOT" && humidityStatus=="HUMID" && gasStatus=="DANGER")
    {
        venti.write(90);
        ventilationStatus = "OPEN";
    }
    else
    {
        venti.write(0);
        ventilationStatus = "CLOSED";
    }
}


    Serial.println("ROOM LIGHT  : "+roomLightStatus);
    Serial.println("GAS BUZZER  : "+buzzerStatus);
    Serial.println("FAN         : "+fanStatus);
    Serial.println("DOOR SERVO  : "+doorStatus);
    Serial.println("VENTILATION : "+ventilationStatus);
    Serial.println("==============================================");




//PUBLISHING SENSOR DATA
  unsigned long now = millis();

if(now-lastMessage>5000)
{
    lastMessage = now;

    String payload;

    //================ TEMPERATURE ================

    payload = "TEMPERATURE : " + String(temp) + " C | STATUS: " + temperatureStatus;
    client.publish("Harshal_SmartHome/sensor/temperature", payload.c_str());

    //================ HUMIDITY ===================

    payload = "HUMIDITY : " + String(hum) + " % | STATUS: " + humidityStatus;
    client.publish("Harshal_SmartHome/sensor/humidity", payload.c_str());

    //================ PRESSURE ===================

    payload = "PRESSURE : " + String(pre) + " Pa | STATUS: " + pressureStatus;
    client.publish("Harshal_SmartHome/sensor/pressure", payload.c_str());

    //================ GAS ========================

    payload = "GAS : " + String(smokesensor) + " ADC | STATUS: " + gasStatus;
    client.publish("Harshal_SmartHome/sensor/gas", payload.c_str());

    //================ MOTION =====================

    payload = "MOTION : " + motionStatus;
    client.publish("Harshal_SmartHome/sensor/motion", payload.c_str());

    //================ DISTANCE ===================

    payload = "DISTANCE : " + String(distance) + " cm | STATUS: " + distanceStatus;
    client.publish("Harshal_SmartHome/sensor/distance", payload.c_str());

    //================ LIGHT ======================

    payload = "LIGHT : " + String(lit) + " lux | STATUS: " + lightStatus;
    client.publish("Harshal_SmartHome/sensor/light", payload.c_str());

    //================ ROOM LIGHT =================

    payload = "ROOM LIGHT : " + roomLightStatus;
    client.publish("Harshal_SmartHome/actuator/light", payload.c_str());

    //================ BUZZER =====================

    payload = "BUZZER : " + buzzerStatus;
    client.publish("Harshal_SmartHome/actuator/buzzer", payload.c_str());

    //================ FAN ========================

    payload = "FAN : " + fanStatus;
    client.publish("Harshal_SmartHome/actuator/fan", payload.c_str());

    //================ DOOR =======================

    payload = "DOOR : " + doorStatus;
    client.publish("Harshal_SmartHome/actuator/door", payload.c_str());

    //================ VENTILATION ================

    payload = "VENTILATION : " + ventilationStatus;
    client.publish("Harshal_SmartHome/actuator/ventilation", payload.c_str());
    
    //=================WIFI=========================

    payload = "WiFi : " + WiFiStatus;
    client.publish("Harshal_SmartHome/network/wifi", payload.c_str());

    //=================MQTT=========================

    payload = "MQTT : " + MQTTStatus;
    client.publish("Harshal_SmartHome/network/mqtt", payload.c_str());

    //=================MODE=========================

    payload = "MODE : " + mode;
    client.publish("Harshal_SmartHome/network/mode", payload.c_str());


}


  //========DISPLAY ENVIRONMENT===========================
  //==================== SCREEN 1 ====================

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);

display.println("SMART HOME V1.0");
display.println("--------------------");

display.println("Temp : " + String(temp) + " C");
display.println("       " + temperatureStatus);

display.println("Hum  : " + String(hum) + " %");
display.println("       " + humidityStatus);

display.println("Pres : " + String(pre) + " Pa");
display.println("       " + pressureStatus);


display.display();
delay(2500);

//==================== SCREEN 2 ====================

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);

display.println("SAFETY");
display.println("--------------------");

display.println("Gas  : " + String(smokesensor));
display.println("       " + gasStatus);

display.println("Motion : " + motionStatus);

display.println("Dist : " + String(distance) + " cm");
display.println("       " + distanceStatus);
display.println("Light : " + String(lit) + " lux");
display.println("       " + lightStatus);

display.display();
delay(2500);
//==================== SCREEN 3 ====================

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);

display.println("ACTUATORS");
display.println("--------------------");

display.println("Light : " + roomLightStatus);

display.println("Fan   : " + fanStatus);

display.println("Door  : " + doorStatus);

display.println("Vent  : " + ventilationStatus);

display.println("Alarm : " + buzzerStatus);

display.display();
delay(2500);
//==================== SCREEN 4 ====================

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);

display.println("NETWORK");
display.println("--------------------");
display.println("WiFi : "+WiFiStatus);
display.println("MQTT : "+MQTTStatus);
display.println("Mode : "+mode);
display.println();
display.println("Smart Home Ready");

display.display();
delay(2500);
}