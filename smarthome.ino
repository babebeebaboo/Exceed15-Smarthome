#include <Servo.h>

/*
   switch -> buzzer//
   ldr -> led
   led//
   ultrasonic -> servo
*/
#define button_switch 1
#define buzzer 10
#define ldr 17
#define led1 4
#define ping_pin 7
#define in_pin 6
#define servo 9
#define ldr_night_value 800

Servo myservo;
int ldr_value = 0; //variable to store LDR values
int floor_length;
int time_open_door = 0;
long duration, cm;
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );
  //servo
  myservo.attach(servo);
  //led
  pinMode(led1, OUTPUT);
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
  // put your main code here, to run repeatedly:
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
  delay(200);
  //servo
  if (cm - floor_length > 2)
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
  if (ldr_value > ldr_night_value)digitalWrite(led1, HIGH);
  else digitalWrite(led1, LOW);
  delay(200);
  //switch buzzer
  if (button_switch == HIGH) //pressed
    tone(buzzer, 1000, 1);

  delay(500);
}
