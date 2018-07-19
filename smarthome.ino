#include <Servo.h>
#include <dht11.h> // https://www.arduinothai.com/article/11/arduino-%E0%B9%80%E0%B8%AD%E0%B8%B2%E0%B9%84%E0%B8%9B%E0%B8%97%E0%B8%B3%E0%B8%AD%E0%B8%B0%E0%B9%84%E0%B8%A3%E0%B9%84%E0%B8%94%E0%B9%89%E0%B8%9A%E0%B9%89%E0%B8%B2%E0%B8%87-%E0%B8%95%E0%B8%AD%E0%B8%99%E0%B8%97%E0%B8%B5%E0%B9%88-6-%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B9%83%E0%B8%8A%E0%B9%89%E0%B8%87%E0%B8%B2%E0%B8%99-dht11-digital-temperature-and-humidity-sensor-2
/*
   switch -> buzzer//
   ldr -> led
   led//
   ultrasonic -> servo
   DHT11
*/
#define button_switch 1
#define buzzer 10
#define ldr 17
#define led1 4
#define ping_pin 7
#define in_pin 6
#define servo 9
#define ldr_night_value 800
#define DHT11_PIN 2

dht11 DHT11;
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
  delay(200);
  
  //dht11
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: Serial.println("OK"); break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);
  delay(500);
}
