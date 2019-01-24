// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Arduino.h>
// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// LED pin
#define LED_PIN   6

//custom boards may override default pin definitions with BLEPeripheral(PIN_REQ, PIN_RDY, PIN_RST)
BLEPeripheral blePeripheral = BLEPeripheral();

// create service
BLEService ledService = BLEService("801e003f-ed28-438f-99bb-1ce471c6873b");


// create switch characteristic
BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("7e858da4-0db8-4098-b30c-f6e695b5b6c6", BLERead | BLEWrite);
BLEUnsignedLongCharacteristic intch = BLEUnsignedLongCharacteristic("dfa9cd6b-1490-4c24-8284-07dc7c1ed39e", BLERead | BLEWrite | BLENotify);
void setup() {
  // Serial.begin(9600);
#if defined (__AVR_ATmega32U4__)
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#endif

  // set LED pin to output mode
  pinMode(LED_PIN, OUTPUT);


  // set advertised local name and service UUID
  blePeripheral.setLocalName("LED2");
  blePeripheral.setDeviceName("Device Name");

  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());
  blePeripheral.setConnectable(true);


  // add service and characteristic

  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);
  blePeripheral.addAttribute(intch);

  //  sd_ble_gap_phy_update(1);
  // begin initialization
  blePeripheral.begin();

  // Serial.println(F("BLE LED Peripheral"));
}
long i=0;
long k=0;
long lastk=0;
void loop() {
  BLECentral central = blePeripheral.central();

  if (central) {
    // central connected to peripheral
    // Serial.print(F("Connected to central: "));
    // Serial.println(central.address());

    while (central.connected()) {
      // central still connected to peripheral
      if (switchCharacteristic.written()) {
        // central wrote new value to characteristic, update LED
        if (switchCharacteristic.value()) {
          // Serial.println(F("LED on"));
          digitalWrite(LED_PIN, HIGH);
        } else {
          // Serial.println(F("LED off"));
          digitalWrite(LED_PIN, LOW);
        }
      }

      i++;
      if(i>3000)
      {
        i=0;
        k++;
      }
      if(k != lastk)
      {
        intch.setValue(k);
        lastk = k;
      }
    }


    // central disconnected
    // Serial.print(F("Disconnected from central: "));
    // Serial.println(central.address());
  }
} 





// #include <SPI.h>
// #include <BLEPeripheral.h>
// BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
// BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// // BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
// BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);

// const int ledPin = 6; // pin to use for the LED

// void setup() {
//   Serial.begin(115200);

//   // set LED pin to output mode
//   pinMode(ledPin, OUTPUT);

//   // set advertised local name and service UUID:
//   blePeripheral.setLocalName("LED");
//   blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

//   // add service and characteristic:
//   blePeripheral.addAttribute(ledService);
//   blePeripheral.addAttribute(switchCharacteristic);

//   // set the initial value for the characeristic:
//   switchCharacteristic.setValue(0);

//   // begin advertising BLE service:
//   blePeripheral.begin();

//   Serial.println("BLE LED Peripheral");
// }

// void loop() {
//   // listen for BLE peripherals to connect:
//   BLECentral central = blePeripheral.central();

//   // if a central is connected to peripheral:
//   if (central) {
//     Serial.print("Connected to central: ");
//     // print the central's MAC address:
//     Serial.println(central.address());

//     // while the central is still connected to peripheral:
//     while (central.connected()) {
//       // if the remote device wrote to the characteristic,
//       // use the value to control the LED:
//       if (switchCharacteristic.written()) {
//         if (switchCharacteristic.value()) {   // any value other than 0
//           Serial.println("LED on");
//           digitalWrite(ledPin, HIGH);         // will turn the LED on
//         } else {                              // a 0 value
//           Serial.println(F("LED off"));
//           digitalWrite(ledPin, LOW);          // will turn the LED off
//         }
//       }
//     }

//     // when the central disconnects, print it out:
//     Serial.print(F("Disconnected from central: "));
//     Serial.println(central.address());
//   }
// }