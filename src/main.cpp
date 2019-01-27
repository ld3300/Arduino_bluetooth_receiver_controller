
#include <Arduino.h>

#define ONTIMER   3000        // How long does button take to turn on (3 seconds for mine)
#define OFFDEBOUNCE 500       // millis to make sure we actually have power loss before killing power

#define POWERPIN  3           // pin to control power to bluetooth receiver. (through transistor)
#define ONSWITCH  5           // pin to "press" the on switch
#define USBSENSE  A0          // pin to sense when USB power has been cut
                              // (there is an array of large capacitors to keep power long enough 
                              // to control turning Bluetooth controller off, which has
                              // batteries, so needs an off command).  This is connected to vin.

void setup() {
  Serial.begin(115200);                     // Start serial output for debugging
  pinMode(POWERPIN, OUTPUT);                // Set power control pin as output
  pinMode(ONSWITCH, OUTPUT);                // set turn on tactile switch bypass as output
  pinMode(USBSENSE, INPUT);                 // set our pin for reading power voltage as input
  pinMode(LED_BUILTIN, OUTPUT);             // set built in led for debugging as output
  digitalWrite(POWERPIN, HIGH);             // Turn on power to the bluetooth receiver
  unsigned long onTimer = millis();         // Take current time for switching on
  digitalWrite(ONSWITCH, HIGH);             // "hold" down the tactile switch turns on receiver
  Serial.print("Powering On");              // Debug
  while(millis() - onTimer <= ONTIMER){     // Keep switch on until we hit on timer
    Serial.print(".");
    delay(500);
  }
  Serial.println("on");                     
  digitalWrite(LED_BUILTIN, HIGH);          // indicate we have turned on device
  digitalWrite(ONSWITCH, LOW);              // we can let go of the switch once we hit on time
  for(int i = 0; i < 4; i ++){              // Do some loops and blinks just to let voltage settle
    delay(500);                
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  uint32_t sensorLevel = analogRead(USBSENSE);  // Check the value of VIN tied to analog pin
  Serial.println(sensorLevel);                  // print for debugging
  if(sensorLevel < 50){                         // when the car is turned off and power lost 
    unsigned long startTime = millis();
    while(!(millis() - startTime > OFFDEBOUNCE));  // hold here to catch short anamolous events.
    while(1){                                   // Running this until capacitance ran out seemed
      Serial.println("powering off");           // to help prevent startup retriggering 
      digitalWrite(LED_BUILTIN, LOW);           
      digitalWrite(POWERPIN, LOW);              // Rather than try to hold the button, I observed in
      delay(100);                               // usage that if bluetooth was on battery and you
      digitalWrite(LED_BUILTIN, HIGH);          // plug in USB it would turn off.  By cycling the
      digitalWrite(POWERPIN, HIGH);             // power low then high we are simulating that effect
      delay(100);                               // this is quicker than trying to hold button for
    }                                           // four seconds which may be too long for current 
  }                                             // capacitor array to hold power to Arduino on.
}
