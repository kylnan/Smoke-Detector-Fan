/* Runs off of an ESP32
  This is a quick and cheap design using an MQ2 and DHT22 sensor
  Goal is to power on different speeds on a lasko box fan based on smoke detection or high temparature
  */
#include <DHT_U.h>
#include <DHT.h>

// DHT setup
#define DHTPIN 32
DHT dht(DHTPIN, DHT22);
// sensor pin declarations
const int mq_AO = 35;
const int mq_DO = 34;

float smokeAnalog;
int   smokeDigital;

//relay declaration
const int relay0 = 5;
const int relay1 = 18;
const int relay2 = 19;
const int relay3 = 21;

void setup() {  
  // put your setup code here, to run once:
  dht.begin();
  
  pinMode(mq_AO, INPUT);
  pinMode(mq_DO, INPUT);
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  delay(500);

  //initialize all relays to be off
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);

  dht.begin();
  delay(20000); // warming up MQ2
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  smokeAnalog = analogRead(mq_AO);
  smokeDigital = digitalRead(mq_DO);
  float temp = dht.readTemperature();
  Serial.print("MQ2 Analog: ");
  Serial.print(smokeAnalog);
  Serial.print(" | Digital: ");
  Serial.print(smokeDigital);

  // Detects smoke and turns on fans highest setting, also checks for high temp
  if(smokeDigital == LOW || temp >= 32){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    Serial.print(" | HIGH SPEED");
  }
  // Speed 2 for moderate temp
  if(temp < 32 && temp >= 30 && smokeDigital == HIGH){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    Serial.print(" | MED SPEED");
  }
  // Speed 1 for more moderate temp
  else if(temp <  30 && temp > 28.00 && smokeDigital == HIGH){
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    Serial.print(" | LOW SPEED");
  }
  // If below 28C and no smoke, leave fan off
  else if(temp <= 28.00 && smokeDigital == HIGH){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    Serial.print(" | FAN OFF");
  }

  Serial.println("");
  Serial.print("Current Temp(C): ");
  Serial.println(temp);
  Serial.println("==================================================");


  delay(250);
}
