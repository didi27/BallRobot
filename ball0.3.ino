#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>

int motorPin1 = D1; //A1-B
int motorPin2 = D2; //A1-A
int motorPin3 = D3; //B1-B
int motorPin4 = D4; //B1-A
int beepPin = D8;

int melody[] = {  1976,1976, 1568,1760, 3136, 1976};
int noteDurations[] = {  4, 8, 8, 4, 4, 4};
String responseHTML = "<!DOCTYPE html><head><meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no' /><style>html, body {height: 100%; margin: 0px;}</style><title>BallBot</title></head><body><div id='top' style='height: 33%;'><a href=''><div id='topl' style='min-width: 33%; height: 100%; float:left'>x</div></a><a href='/forwards'><div id='topm' style='width: 33%; height: 100%; float:left; background-color: blue'><h1>&#8593;</h1></div></a><a href='/flipout'><div id='topr' style='width: 33%; height: 100%; float:left'>FLIPOUT</div></a></div><div id='middle' style='height: 33%'><a href='/turnleft'><div id='middlel' style='width: 33%; height: 100%; float:left; background-color: blue'><h1>&lt;X</h1></div></a><a href='/beep'><div id='middlem' style='width: 33%; height: 100%; float:left; background-color: red'></div></a><a href='/turnright'><div id='middler' style='width: 33%; height: 100%; float:left; background-color: blue'><h1>&gt;</h1></div></a></div><div id='bottom' style='height: 33%'><a href=''><div id='bottoml' style='width: 33%; height: 100%; float:left'></div></a><a href='/backwards'><div id='bottomm' style='width: 33%; height: 100%; float:left; background-color: blue'><h1>v</h1></div></a><a href=''><div id='bottomr' style='width: 33%; height: 100%; float:left;'></div></a></div></body></html>";

byte fullspeed = 255;
byte halfspeed = 120;
byte zerospeed = 0;

const byte DNS_PORT = 80;
IPAddress apIP(192, 168, 1, 1);
const char *ssidbot = "Dude";
const char *password = "thereisnospoon";
DNSServer dnsServer;
ESP8266WebServer server(80);
void robotBeep(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(beepPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(beepPin);
  }
}
 void stopall(){
   digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, 0);
 }
void forwards(int duration)
{
  digitalWrite(motorPin1, fullspeed);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, fullspeed);
  digitalWrite(motorPin4, 0);
  delay(duration*0.6);
  digitalWrite(motorPin1, halfspeed);
  digitalWrite(motorPin3, halfspeed);
  delay(duration*0.4);
  stopall();
  Serial.write("forwards "+ duration);
}
void backwards(int duration)
{
  digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, fullspeed);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, fullspeed);
  delay(duration);
  stopall();
  Serial.write("backwards "+ duration);
}
void pause(int duration)
{
   digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, 0);
  delay(duration); 
  Serial.write("pause "+ duration);
 }

 void turnright(int duration)
{
  digitalWrite(motorPin1, fullspeed);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, 0);
  delay(duration); 
  stopall();
}
void turnleft(int duration)
{
  digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, fullspeed);
  digitalWrite(motorPin4, 0);
  delay(duration); 
  stopall();
}
void flip(int duration)
{
  digitalWrite(motorPin1, fullspeed);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, fullspeed);
  digitalWrite(motorPin4, 0);
  delay(duration); 
  stopall();
}
void handleRoot() {
  server.send(200, "text/html", responseHTML);
}
void handleforewards(){
forwards(750);
  server.send(200, "text/html", responseHTML);
}
void handlebackwards(){
backwards(750);
  server.send(200, "text/html", responseHTML);
}
void handleleft(){
turnleft(100);
  server.send(200, "text/html", responseHTML);
}
void handleright(){
turnright(100);
  server.send(200, "text/html", responseHTML);
}
void handleflipout(){
turnleft(2050);
  server.send(200, "text/html", responseHTML);
}
void handlebeep(){
robotBeep();
  server.send(200, "text/html", responseHTML);
}
void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);

WiFi.softAP(ssidbot, password);
WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", handleRoot);
  server.on("/forwards", handleforewards);
  server.on("/backwards", handlebackwards);
   server.on("/turnleft", handleleft);
    server.on("/turnright", handleright);
  server.on("/flipout", handleflipout);
  server.on("/beep", handlebeep);
  server.onNotFound([]() {
  server.send(200, "text/html", responseHTML);
  });
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
dnsServer.processNextRequest();
server.handleClient();

}


