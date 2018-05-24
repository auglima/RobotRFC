#include <ESP8266WiFi.h>
#include "Roda.h"

#define WHEEL_LEFT_A          4
#define WHEEL_LEFT_B          5
#define WHEEL_RIGHT_A     13
#define WHEEL_RIGHT_B     15
#define WEAPON                     14

#define BACKWARD  1
#define FORWARD    0
#define YES                 1
#define NO                  0



//VARIABLES
const char *ssid = "MyUglyRobot";
const char *password = "12345678a";

Wheel leftWheel   (WHEEL_LEFT_A    , WHEEL_LEFT_B);
Wheel rightWheel(WHEEL_RIGHT_A , WHEEL_RIGHT_B);

int _pwm                   = 0;
boolean _stop           = YES;
boolean  _direction  = FORWARD;
boolean _left             = NO;
boolean _right          = NO;
boolean _weapon     = NO;


//STARTING WEB SERVER/CLIENT
WiFiServer server(80);
WiFiClient client;


//ROBOT FUNCTIONS
void stopp() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("break command...");
  leftWheel.stopp();
  rightWheel.stopp(); 
}

void forward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("forward command...");
  leftWheel.forward(_pwm);
  rightWheel.forward(_pwm);
}

void backward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("backward command...");
  leftWheel.backward(_pwm);
  rightWheel.backward(_pwm);
}

void turnLeftForward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("turnLeftForward command...");
  leftWheel.forward((int)(_pwm - (_pwm * 0.7)));
  rightWheel.forward(_pwm);
}

void turnRightForward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("turnRightForward command...");
  leftWheel.forward(_pwm);
  rightWheel.forward((int)(_pwm - (_pwm * 0.7)));
}

void turnLeftBackward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("turnLeftBackward command...");
  leftWheel.backward((int)(_pwm - (_pwm * 0.7)));
  rightWheel.backward(_pwm);
}

void turnRightBackward() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("turnRightBackward command...");
  leftWheel.backward(_pwm);
  rightWheel.backward((int)(_pwm - (_pwm * 0.7)));
}

void rotateLeft() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("rotateLeft command...");
  leftWheel.backward(200);
  rightWheel.forward(200);
}

void rotateRight() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("rotateRight command...");
  leftWheel.forward(200);
  rightWheel.backward(200);
}

void startWeapon() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("startWeapon command...");
  digitalWrite(WEAPON, HIGH);
}

void stopWeapon() {
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("stopWeapon command...");
  digitalWrite(WEAPON, LOW);
}



void setup() {
  pinMode(WEAPON, OUTPUT);
  delay(10);

  Serial.begin(115200);
  
  IPAddress staticIP(192, 168, 4, 2);     // IP set to Static
  IPAddress gateway(192, 168, 4, 1);    // gateway set to Static
  IPAddress subnet(255, 255, 255, 0);  // subnet set to Static
  
  WiFi.mode(WIFI_AP);  // Working mode only as Acess Point 

  WiFi.softAP(ssid, password, 3, 0);  //rede, senha, canal (1 a 11), visivel
  WiFi.config(staticIP, gateway, subnet);

  server.begin();
  Serial.println();
  Serial.print("AP com IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Servidor Iniciado!\n");
}

void loop() {
  client = server.available();

  if(!client){ return; }

  while(!client.available()){ delay(1); }
  
  String msg = client.readStringUntil('\r');
  msg = msg.substring(msg.indexOf("/")+1, msg.indexOf("HTTP")-1);
    
  client.print("HTTP://1.1 200 OK\n\n");
  client.flush();

  if (msg.length() == 8) {

    _pwm          = msg.substring(0,3).toInt();
    _stop           = msg.charAt(3) - '0';
    _direction   = msg.charAt(4) - '0';
    _left             = msg.charAt(5) - '0';
    _right          = msg.charAt(6) - '0';
    _weapon     = msg.charAt(7) - '0';

    // estados para o carro parado
    if (_stop == YES) {
      stopp();  // parar

      // rotacionar      
      if (_left == YES && _right == NO) { rotateLeft(); }
      else if (_left == NO && _right == YES) { rotateRight(); }
    }
    else {
      // mover para frente/tras
      if (_left == NO && _right == NO) {
        if (_direction == FORWARD) { forward(); } else { backward(); }
      }
      // curvas
      else {
        // curva para frente
        if (_direction == FORWARD) {
          if (_left == YES && _right == NO) { turnLeftForward(); }
          else if (_left == NO && _right == YES) { turnRightForward(); }
        }
        else {
          if (_left == YES && _right == NO) { turnLeftBackward(); }
          else if (_left == NO && _right == YES) { turnRightBackward(); }
        }
      }
    }
    
  }

  client.print("HTTP://1.1 200 OK\n\n");
  client.flush();
  delay(5);
}
