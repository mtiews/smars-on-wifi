
#include <ESP8266WiFi.h>
#include "Automode.h"
#include "Webcontrol.h"

/* Enable the following line to switch to Station Mode 
 * and connect to an existing WIFI 
 */
// #define ACTIVATE_STATION_MODE

/* WIFI Settings
 * put your wifi ssid/password here or put them to secret.h
 * to .gitignore your secrets in secret.h
 */
#define WIFI_SSID "Smars1"
#define WIFI_PASSWORD "Smars1000"
#ifndef WIFI_SSID
  #include secret.h
#endif

/* Settings Right Motor */
#define MOTOR_R_PIN1 D1
#define MOTOR_R_PIN2 D2
#define MOTOR_R_PWM_VALUE 1000

/* Settings Left Motor */
#define MOTOR_L_PIN1 D3
#define MOTOR_L_PIN2 D4
#define MOTOR_L_PWM_VALUE 1000

/* Settings Sonic Sensor */
#define SENSOR_SONIC_TRIG_PIN D5
#define SENSOR_SONIC_ECHO_PIN D6
#define OBSTACLE_DISTANCE_IN_CM 10

/* Protos for Handler functions */
boolean sensorSonicObstacleInSight(); 
void moveForward();
void moveLeft();
void moveRight(); 
void moveBackward();
void enableAutomode();
void moveStop();

/* Automode */
Automode automode(
  sensorSonicObstacleInSight, 
  moveForward, 
  moveLeft, 
  moveRight, 
  moveStop
);

/* Webcontrol */
Webcontrol webcontrol(
  80,
  moveForward, 
  moveLeft, 
  moveRight,
  moveBackward, 
  moveStop,
  enableAutomode
);

/* Vars */
boolean autoModeEnabled = false;

void setup() {
  Serial.begin(115200);
  setupMotors();
  setupSonicSensor();
  setupWifi();
  webcontrol.begin();
}

void loop() {
  webcontrol.handleClientCommands();
  if(autoModeEnabled) {
    boolean moving = automode.nextMove();
    delay(100);
  }
}

void enableAutomode() {
  automode.reset();
  autoModeEnabled = true;
}

/* WIFI */

void setupWifi() {
#ifdef ACTIVATE_STATION_MODE
  Serial.println("Starting in Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#else
  Serial.println("Starting in Access Point Mode");
  boolean status = WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  if(!status) {
    Serial.println("Setting up Access Point Mode failed! :-(");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
#endif
}

/* "Moves" */

void moveForward() {
  moveLeftTrack(1);
  moveRightTrack(1);
}

void moveBackward() {
  moveLeftTrack(-1);
  moveRightTrack(-1);
}

void moveLeft() {
  moveLeftTrack(-1);
  moveRightTrack(1);
}

void moveRight() {
  moveLeftTrack(1);
  moveRightTrack(-1);
}

void moveStop() {
  autoModeEnabled = false;
  moveLeftTrack(0);
  moveRightTrack(0);
}

/* Sonic Sensor */

void setupSonicSensor() {
  pinMode(SENSOR_SONIC_TRIG_PIN, OUTPUT);
  pinMode(SENSOR_SONIC_ECHO_PIN, INPUT);
}

boolean sensorSonicObstacleInSight() {
  digitalWrite(SENSOR_SONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_SONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_SONIC_TRIG_PIN, LOW);
  long duration = pulseIn(SENSOR_SONIC_ECHO_PIN, HIGH, 3000);
  if(duration == 0) {
    return false;
  }
  long distanceCM = duration/58.2;
  
  return distanceCM < OBSTACLE_DISTANCE_IN_CM;
}

/* Motor / Track steering */

void setupMotors() {
  pinMode(MOTOR_R_PIN1, OUTPUT);
  pinMode(MOTOR_R_PIN2, OUTPUT);
  pinMode(MOTOR_L_PIN1, OUTPUT);
  pinMode(MOTOR_L_PIN2, OUTPUT);

  digitalWrite(MOTOR_R_PIN1, LOW);
  digitalWrite(MOTOR_R_PIN2, LOW);
  digitalWrite(MOTOR_L_PIN1, LOW);
  digitalWrite(MOTOR_L_PIN2, LOW);
}

void moveRightTrack(int direction) {
  if(direction > 0) {
    analogWrite(MOTOR_R_PIN1, MOTOR_R_PWM_VALUE);
    digitalWrite(MOTOR_R_PIN2, LOW);
  } else if(direction < 0) {
    digitalWrite(MOTOR_R_PIN1, LOW);
    analogWrite(MOTOR_R_PIN2, MOTOR_R_PWM_VALUE);
  } else {
    digitalWrite(MOTOR_R_PIN1, LOW);
    digitalWrite(MOTOR_R_PIN2, LOW);  
  }
}

void moveLeftTrack(int direction) {
  if(direction > 0) {
    digitalWrite(MOTOR_L_PIN1, LOW);
    analogWrite(MOTOR_L_PIN2, MOTOR_L_PWM_VALUE);
  } else if(direction < 0) {
    analogWrite(MOTOR_L_PIN1, MOTOR_L_PWM_VALUE);
    digitalWrite(MOTOR_L_PIN2, LOW);
  } else {
    digitalWrite(MOTOR_L_PIN1, LOW);
    digitalWrite(MOTOR_L_PIN2, LOW);
  }
}
