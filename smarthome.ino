#include <Servo.h>
/*
   switch -> buzzer//
   ldr -> led
   led//
   ultrasonic -> servo
   DHT11
*/
/*

   switch 2
   buzzer 10
   ldr 17(a3)
   led 4
   led 5
   ultrasonic 7 (trigger),6 (echo)
   servo 9 yellow , gnd brown, vcc red
   DHT11 12
*/
#define button_switch 2
#define buzzer 11
#define ldr 17
#define led1 4
#define led2 5
#define ping_pin 7
#define in_pin 6
#define servo 9
#define ldr_night_value 800

#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11 // DHT 11 
DHT dht(DHTPIN, DHTTYPE);


Servo myservo;

int ldr_value = 0; //variable to store LDR values
int floor_length;
int time_open_door = 0;
long duration, cm;

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
void setup() {
  Serial.begin( 9600 );

  //servo
  myservo.attach(servo);

  //led
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  //buzzer
  pinMode(buzzer , OUTPUT);

  //ultrasonic
  pinMode(ping_pin, OUTPUT);
  digitalWrite(ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping_pin, LOW);
  pinMode(in_pin, INPUT);
  duration = pulseIn(in_pin, HIGH);
  floor_length = microsecondsToCentimeters(duration);

  pinMode(button_switch, INPUT);
  pinMode(ldr, INPUT);

}

void loop() {
  //ultrasonic
  pinMode(ping_pin, OUTPUT);
  digitalWrite(ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping_pin, LOW);
  pinMode(in_pin, INPUT);
  duration = pulseIn(in_pin, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.println("cm");


  //servo
  if (cm - floor_length < -2)
  {
    myservo.write(90);
    time_open_door = millis();
  }
  else
    myservo.write(0);
  if (millis() - time_open_door > 5000)
    myservo.write(0);
  delay(200);

  //ldr
  ldr_value = analogRead(ldr); //reads the LDR values
  Serial.print("ldr_value: ");
  Serial.println(ldr_value); //prints the LDR values to serial monitor
  if (ldr_value > ldr_night_value)
    digitalWrite(led1, HIGH);
  else digitalWrite(led1, LOW);


  //switch buzzer
  if (digitalRead(button_switch) == LOW) //pressed
  {
    analogWrite(buzzer, HIGH);
    Serial.println("Switch Pressed");
  }
  else {
    analogWrite(buzzer, LOW);
    Serial.println("Switch NOT Pressed");
  }


  //DHT
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
  if (h > 70)
    digitalWrite(led2, HIGH);
  else
    digitalWrite(led2, LOW);
  delay(500);
}
