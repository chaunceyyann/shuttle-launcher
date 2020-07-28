
/*
 Controlling two dc motor position using Serial
 by Chauncey Yan
 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
*/

#include <SoftwareSerial.h>
#include <Servo.h>


// Bluetooth TX -> Arduino 11
const int BLUETOOTH_TX = 11;
// Bluetooth RX -> Arduino 6
const int BLUETOOTH_RX = 6;

int throttle = 0; // 0 - 10
int steering = 0; // -5 - 5
int shuttle_speed = 0;
int delta = 0;
SoftwareSerial phone(BLUETOOTH_TX, BLUETOOTH_RX);

Servo escL;
Servo escR;

int escLPin = 4;
int escRPin = 5;
int minPulseRate = 1000;
int maxPulseRate = 2000;
int throttleChangeDelay = 100;

// Setup code runs once after program starts.
void setup() {
    // Start serial monitor at 9600 bps.
    Serial.begin(9600);
    Serial.setTimeout(500);

    // Start bluetooth serial at 9600 bps.
    phone.begin(9600);

    // delay just in case bluetooth module needs time to "get ready".
    delay(100);

    // Attach the the servo to the correct pin and set the pulse range
    escL.attach(escLPin, minPulseRate, maxPulseRate);
    escR.attach(escRPin, minPulseRate, maxPulseRate);

    // Write a minimum value (most ESCs require this correct startup)
    escL.write(0);
    escR.write(0);

    Serial.println("setup complete");
}

// Put your main code here, to run repeatedly:
void loop() {
    if (phone.available()) {
      int command = phone.parseInt();
      Serial.print("Receive command from phone: ");
      Serial.println(command);
      switch (command) {
        case 0:
          throttle = 0;
          steering = 0;
          break;
        case 1:
          throttle = throttle>9000?throttle:throttle+1000;
          break;
        case 2:
          throttle = throttle<1000?throttle:throttle-1000;
          break;
        case 3:
          steering = steering>4?steering:steering+1;
          break;
        case 4:
          steering = steering<-4?steering:steering-1;
          break;
        case 5:
          break;
        default:
          break;
      }
      shuttle_speed = map(round(sqrt(throttle)), 0, 100, 14, 50);
      delta = map(steering, -5, 5, -5, 5);
      Serial.print("Setting shuttle_speed to (14-30): ");
      Serial.println(shuttle_speed);
      Serial.print("Setting delta to (-5 - 5): ");
      Serial.println(delta);
    }
    escL.write(shuttle_speed+delta);
    escR.write(shuttle_speed-delta);
    
    // Send string from serial command line to the phone. This will alert the user.
//    if (Serial.available()) {
        // Serial.write("send: ");
        // String str = Serial.readString();
        // phone.sendMessage(str); // phone.sendMessage(str) sends the text to the phone.
        // Serial.print(str);
        // Serial.write('\n');

        
//        int val = Serial.parseInt();
//        if (val != 0)
//        {
//          int throttle = val % 100;
//          steering = 49;
//          
//            
//          int shuttle_speed = map(throttle%50+50, 49, 99, 10, 30);
//          int delta = map(steering, 0, 98, -5, 5);
//                  
//          Serial.print("Setting shuttle_speed to (10-30): ");
//          Serial.println(shuttle_speed);
//          Serial.print("Setting delta to (-5 - 5): ");
//          Serial.println(delta);
//          escL.write(shuttle_speed+delta);
//          escR.write(shuttle_speed-delta);
//
//        }
//    }
}
