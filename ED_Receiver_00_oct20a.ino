// Receiver Node  ||  Engineering Design group 271


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9,8);				           // nRF24L01(+) receiver connected to pin 9 and 10
RF24Network network(radio); 	      // define network

const uint16_t nodeReceiver = 00;   // Address of this node in Octal format ( 04,031, etc)
//other nodes:  nodeTransmitterA = 01, nodeTransmitterB = 02, nodeTransmitterC = 03

const int triggerDistance = 50;     // Distance (in cm) of the Athlete to the sensor at which a signal should be triggered ### can be adapted ###
//const int triggerPin = 6;           // Trigger pin ultrasonic sensor
//const int echoPin = 7;              // Echo pin ultrasonic sensor
const int buzzerPin = 5;           // Buzzer to arduino pin 5

float frequency = 200;              // Starting frequency of buzzer sound


bool passed_A = false;              // Boolean to track which nodes the athlete has passed
bool passed_B = false;
bool passed_C = false;

bool mariotheme = true;             // Can be turned off if you are going insane because of it

//struct distances = {
//int dist;
//char sensor;
//};

void setup()
{
  pinMode(buzzerPin, OUTPUT); // Set buzzer - pin 5 as an output

  // startup sound, because we can (figured all the frequencies out last year for a personal project hahaha)
  if (mariotheme == true)
  {
    tone(buzzerPin, 659.2, 100);
    delay(100);
    tone(buzzerPin, 587.3, 100);
    delay(100);
    tone(buzzerPin, 369.9, 180);
    delay(180);
    tone(buzzerPin, 415.3, 200);
    delay(200);
    tone(buzzerPin, 554.3, 100);
    delay(100);
    tone(buzzerPin, 493.8, 100);
    delay(100);
    tone(buzzerPin, 293.6, 180);
    delay(180);
    tone(buzzerPin, 329.6, 200);
    delay(200);

    tone(buzzerPin, 493.8, 100);
    delay(100);
    tone(buzzerPin, 440, 100);
    delay(100);
    tone(buzzerPin, 277.1, 180);
    delay(180);
    tone(buzzerPin, 329.6, 200);
    delay(200);
    tone(buzzerPin, 440, 400);
    delay(2000);
  }

  Serial.begin(57600);

  SPI.begin();
  radio.begin();
  network.begin(90, nodeReceiver); // (channel, node_adress)

  Serial.println("Sensor network receive start...");
}


void loop()
{
  network.update();

  while ( network.available() ) {

    RF24NetworkHeader header;

    int distance_cm;
    network.read(header, &distance_cm, sizeof(distance_cm));

    // here type 'A' means from node 01, 'B' from 02 and 'C' from 03
    if (distance_cm < triggerDistance && distance_cm !=0) {
      if (header.type == 'A') {
        passed_A = true;
      } else if (header.type == 'B') {
        passed_B = true;
      } else if (header.type == 'C') {
        passed_C = true;
      }
    }
    // booleans passed_A etc. can be used to take measures when for instance passed_B is true but passed_A is still false (by adding code if necessary)


    if (passed_C) {
      frequency = 800;
    } else if (passed_B) {
      frequency = 600;
    } else if (passed_A) {
      frequency = 1000;
    }

    tone(buzzerPin, frequency);
    delay(50);
    noTone(buzzerPin);
    delay(50);
    // possibly in this format if necessary: tone(pin, frequency, duration)
    // so now it is programmed to only increase frequency with increments of 200 Hz every time the athlete passes another sensor

  }
}
