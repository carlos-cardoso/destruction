#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
//#include <ESP8266HTTPclient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>


const char* ssid = "robot_wifi";
const char* password = "#amialive#3.14";
ESP8266WebServer server(80);
// Set your Static IP address
//IPAddress local_IP(192, 168, 1, 184);
IPAddress local_IP(192, 168, 43, 240);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotorRight = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *myDet1 = AFMS.getMotor(4);

Servo servo;

void detonate(){
  Serial.println("5...");
  delay(1000);
  Serial.println("4...");
  delay(1000);
  Serial.println("3...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("1...");
  delay(1000);

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myDet1->setSpeed(255);
  myDet1->run(FORWARD);
  // turn on motor
  delay(5);
  myDet1->run(RELEASE);
  delay(5);
  myDet1->setSpeed(0);
}

void start_motor_shield(){
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
 }

void handleCar() {
 String message = "";
 int BtnValue = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="a")
    {
      String s = server.arg(i);
      BtnValue = s.toInt();
    }
    Serial.println(server.argName(i) + ": " + server.arg(i) + "\n");
  }

  switch (BtnValue) {
   case 1: // sola donuş
      //motorSpeed(900,LOW,LOW,1023,HIGH,LOW);
      //SolSinyal = 1;
      //digitalWrite(Led1_pin,HIGH);
      break;
   case 2: // düz ileri
     // motorSpeed(1023,HIGH,LOW,1023,HIGH,LOW);
     //SolSinyal = 0;
     //SagSinyal = 0;
     //digitalWrite(Led1_pin,LOW);
     //digitalWrite(Led2_pin,LOW);
     myMotorRight->setSpeed(100);
     myMotorRight->run(FORWARD);
     myMotorLeft->setSpeed(100);
     myMotorLeft->run(FORWARD);
     //delay(1000);
     // turn on motor

     break;
  case  3:// saga donuş
     //motorSpeed(1023,HIGH,LOW,900,LOW,LOW);
     //SagSinyal = 1;
     //digitalWrite(Led2_pin,HIGH);
     break;
  case  4:// tam sola donuş
     //motorSpeed(900,LOW,HIGH,900,HIGH,LOW);
     //SolSinyal = 1;
     //digitalWrite(Led1_pin,HIGH);
     myMotorRight->setSpeed(100);
     myMotorRight->run(FORWARD);
     myMotorLeft->setSpeed(0);
     myMotorLeft->run(FORWARD);

     break;
  case 5: // stop
   //motorSpeed(0,LOW,LOW,0,LOW,LOW);
   //SolSinyal = 0;
   //SagSinyal = 0;
   //digitalWrite(Led1_pin,LOW);
   //digitalWrite(Led2_pin,LOW);
   //myMotor->setSpeed(0);
   //myMotor->run(FORWARD);
   // turn on motor
   myMotorLeft->run(RELEASE);
   myMotorRight->run(RELEASE);

   break;
  case  6://
     //motorSpeed(900,HIGH,LOW,900,LOW,HIGH);
     //SagSinyal = 1;
    //digitalWrite(Led2_pin,HIGH);
    myMotorLeft->setSpeed(100);
    myMotorLeft->run(FORWARD);
    myMotorRight->setSpeed(0);
    myMotorRight->run(FORWARD);
    break;
  case 7://sol geri
    //motorSpeed(900,LOW,LOW,1023,LOW,HIGH);
    break;
  case 8:// tam geri
    //motorSpeed(900,LOW,HIGH,900,LOW,HIGH);
     myMotorRight->setSpeed(100);
     myMotorRight->run(BACKWARD);
     myMotorLeft->setSpeed(100);
     myMotorLeft->run(BACKWARD);

    break;
  case 9:// sag geri
    //motorSpeed(1023,LOW,HIGH,900,LOW,LOW);
    break;

  case 99:// sag geri
    Serial.println("DETONATE");
    myMotorLeft->run(RELEASE);
    myMotorRight->run(RELEASE);
    detonate();
    break;

  case 80://Servo
    myMotorLeft->run(RELEASE);
    myMotorRight->run(RELEASE);

    Serial.println("SERVO");
    for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }

    servo.write(90);

  default:
    break;
  }
  /*
  if (BtnValue > 7)
  {
    ArkaLamba = 1;
    SolSinyal = 1;
    SagSinyal = 1;
    digitalWrite(Led1_pin,HIGH);
    digitalWrite(Led2_pin,HIGH);
    digitalWrite(Led4_pin,HIGH);
  }
  else
  {
    ArkaLamba = 0;
    digitalWrite(Led4_pin,LOW);
  }
 */
  message += "<html> <head> <title>Robot Control</title><head>";
  message += "<body><h3>Wifi Robot NodeMCU  Web Server</h1>";
  message += "<table> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=1\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "<td><p><a href=\"/car?a=2\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">^</button></a></p> ";
  message += "<td><p><a href=\"/car?a=3\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=4\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> < </button></a></p> ";
  message += "<td><p><a href=\"/car?a=5\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> X </button></a></p> ";
  //message += "<td><p><a href=\"/car?a=5\"><button style=\"width:100;height:100;font-size:40px;\" class=\"button\">Stop</button></a></p> ";
  message += "<td><p><a href=\"/car?a=6\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> > </button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=7\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<td><p><a href=\"/car?a=8\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">v</button></a></p> ";
  message += "<td><p><a href=\"/car?a=9\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "</table> ";
  message += "<p><a href=\"/car?a=99\"><button style=\"width:300;height:100;font-size:40px;\" class=\"button\">DETONATE</button></a></p> ";
  message += "<p><a href=\"/car?a=5\"><button style=\"width:300;height:100;font-size:40px;\" class=\"button\">STOP</button></a></p> ";
  message += "<p><a href=\"/car?a=80\"><button style=\"width:300;height:100;font-size:40px;\" class=\"button\">SERVO</button></a></p> ";
  message += "</body></html>";
  server.send(200, "text/html", message);
 }

void setup(){
  Serial.begin(9600);
  Wire.begin(D1, D2);// join i2c bus with SDA=D1 and SCL=D2 of NodeMCU
  delay(10);
  start_motor_shield();
  //detonate();
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)){//}, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Print the IP address of the device:
  Serial.println(WiFi.localIP());

  servo.attach(2); //D4

  server.on("/", handleRoot);
  server.on("/car", handleCar);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  //myMotor->run(RELEASE);
}
