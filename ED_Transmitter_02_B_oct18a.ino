 
// Node B  ### Engineering Design group 271 

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(9, 8);                // nRF24L01 (CE,CSN)
RF24Network network(radio);       // Include the radio in the network

const uint16_t this_node = 02;    // Address of this node in Octal format ( 04,031, etc)
const uint16_t nodeReceiver = 00;
   
const int triggerPin = 6;         // Trigger pin
const int echoPin = 7;            // Echo pin

long interval = 5;               // Send message every 5 milliseconds   ### can be adapted ###
long duration;
long time_last_message;


void setup() {

  pinMode(triggerPin,OUTPUT);     // Trigger is an output pin
  pinMode(echoPin,INPUT);         // Echo is an input pin
  
  Serial.begin(57600);            // Serial Output
  
  time_last_message = millis();
 
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node_address)
}

void loop() {

  network.update();
  
  long time_now = millis();
  
   
  if( time_now - time_last_message >= interval )
  {
    digitalWrite(triggerPin, LOW);                   
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);          // Trigger pin to HIGH
    delayMicroseconds(10);                   // 10us high 
    digitalWrite(triggerPin, LOW);           // Trigger pin to HIGH
   
    duration = pulseIn(echoPin,HIGH);        // Waits for the echo pin to get high and returns the Duration in microseconds

    long distance_cm = duration*0.034/2;     // Function to calculate the distance
  
    Serial.print("Distance = ");             // Output to serial port
    Serial.print(distance_cm);
    Serial.println(" cm");
   
    network.update();
    RF24NetworkHeader header(nodeReceiver, 'B');                          // (Address where the data is going, Node from which it is sent)
    bool ok = network.write(header, &distance_cm, sizeof(distance_cm));   // Send the data
    
    if( ok )
    {
      Serial.println("Message sent.");
    } else
    {
      Serial.println("ERROR.");
    }
    
    time_last_message = millis();
  }
}


