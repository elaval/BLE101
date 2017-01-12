/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <CurieBLE.h>



/*
   This sketch example partially implements the standard Bluetooth Low-Energy Battery service.
   For more information: https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx

   You can try it with these free mobile apps:

   iPhone: http://appstore.com/ble-101
   Android: http://play.google.com/store/apps/details?id=com.firstmakers.ble101

*/


BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService arduinoService("6fbee06f-b902-4ac1-bf99-8d4a9b76db8f"); // create service

// BLE  Characteristics
BLECharacteristic pin0Char("AA00", BLERead | BLENotify,2);     // remote clients will be able to
BLECharacteristic pin1Char("AA01", BLERead | BLENotify,2);     // remote clients will be able to
BLECharacteristic pin2Char("AA02", BLERead | BLENotify,2);     // remote clients will be able to
BLECharacteristic pin3Char("AA03", BLERead | BLENotify,2);     // remote clients will be able to
BLECharacteristic pin4Char("AA04", BLERead | BLENotify,2);     // remote clients will be able to
BLECharacteristic pin5Char("AA05", BLERead | BLENotify,2);     // remote clients will be able to

BLECharacteristic mychars[6] = {pin0Char, pin1Char, pin2Char, pin3Char, pin4Char, pin5Char };

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic pinD0Char("DD00", BLERead | BLEWrite );
BLECharCharacteristic pinD1Char("DD01", BLERead | BLEWrite );
BLECharCharacteristic pinD2Char("DD02", BLERead | BLEWrite );
BLECharCharacteristic pinD3Char("DD03", BLERead | BLEWrite );
BLECharCharacteristic pinD4Char("DD04", BLERead | BLEWrite );
BLECharCharacteristic pinD5Char("DD05", BLERead | BLEWrite );
BLECharCharacteristic pinD6Char("DD06", BLERead | BLEWrite );
BLECharCharacteristic pinD7Char("DD07", BLERead | BLEWrite );
BLECharCharacteristic pinD8Char("DD08", BLERead | BLEWrite );
BLECharCharacteristic pinD9Char("DD09", BLERead | BLEWrite );
BLECharCharacteristic pinD10Char("DD10", BLERead | BLEWrite );
BLECharCharacteristic pinD11Char("DD11", BLERead | BLEWrite );
BLECharCharacteristic pinD12Char("DD12", BLERead | BLEWrite );
BLECharCharacteristic pinD13Char("DD13", BLERead | BLEWrite );

BLECharCharacteristic myDchars[14] = {pinD0Char,pinD1Char,pinD2Char,pinD3Char,pinD4Char,pinD5Char,pinD6Char,pinD7Char,pinD8Char,pinD9Char,pinD10Char,pinD11Char,pinD12Char,pinD13Char };

    
// get notifications if this characteristic changes

long previousMillis = 0;  // last time the battery level was checked, in ms

int oldDValues[14];  // previous values for each digital pin
int oldAValues[6];  // previous values for each analog pin

const int pinD0 = 0; 
const int pinD1 = 1; 
const int pinD2 = 2; 
const int pinD3 = 3; 
const int pinD4 = 4; 
const int pinD5 = 5; 
const int pinD6 = 6; 
const int pinD7 = 7; 
const int pinD8 = 8; 
const int pinD9 = 9; 
const int pinD10 = 10; 
const int pinD11 = 11; 
const int pinD12 = 12; 
const int pinD13 = 13; 
const int pinA0 = A0;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;
const int pinA4 = A4;
const int pinA5 = A5;

int myDPins[14] = {pinD0, pinD1, pinD2, pinD3, pinD4, pinD5, pinD6, pinD7, pinD8, pinD9, pinD10, pinD11, pinD12, pinD13};
int myAPins[6] = {pinA0, pinA1, pinA2, pinA3, pinA4, pinA5};



void setup() {
  Serial.begin(9600);    // initialize serial communication
  // Intial previous values to 0 (used to detech changes)
  for (int i=0; i <= 13; i++){
    oldDValues[i] = 0;
    pinMode(myDPins[i], OUTPUT);
    digitalWrite(myDPins[i], LOW);
  } 
  
  for (int i=0; i <= 5; i++){
    oldAValues[i] = 0;
  } 
  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet */
  blePeripheral.setLocalName("BLE101");
  blePeripheral.setAdvertisedServiceUuid(arduinoService.uuid());  // add the service UUID
  blePeripheral.addAttribute(arduinoService);   // Add the BLE Battery service


  // Configure analog ports
  for (int i=0; i <= 5; i++){
    blePeripheral.addAttribute(mychars[i]);

    const unsigned char pinCharArray[2] = { (char)0, (char)0 };
    mychars[i].setValue(pinCharArray, 2);
  } 


  // Configure digital ports
  for (int i=0; i <= 13; i++) {
    blePeripheral.addAttribute(myDchars[i]);
    myDchars[i].setValue(0);
  } 
  myDchars[13].setValue(1);

  myDchars[0].setEventHandler(BLEWritten, listener0);
  myDchars[1].setEventHandler(BLEWritten, listener1);
  myDchars[2].setEventHandler(BLEWritten, listener2);
  myDchars[3].setEventHandler(BLEWritten, listener3);
  myDchars[4].setEventHandler(BLEWritten, listener4);
  myDchars[5].setEventHandler(BLEWritten, listener5);
  myDchars[6].setEventHandler(BLEWritten, listener6);
  myDchars[7].setEventHandler(BLEWritten, listener7);
  myDchars[8].setEventHandler(BLEWritten, listener8);
  myDchars[9].setEventHandler(BLEWritten, listener9);
  myDchars[10].setEventHandler(BLEWritten, listener10);
  myDchars[11].setEventHandler(BLEWritten, listener11);
  myDchars[12].setEventHandler(BLEWritten, listener12);
  myDchars[13].setEventHandler(BLEWritten, listener13);

  /* Now activate the BLE device.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */
  blePeripheral.begin();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // Serial.print("Looping"); // print it
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    //blePeripheral.poll();
    
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(13, HIGH);
    char highValue = HIGH;
    myDchars[13].setValue(highValue);
    haschangedDigital(13);

    // check the battery level every 200ms
    // as long as the central is still connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;

        for (int i=0; i <= 5; i++){
          if (haschangedAnalog(i)) {
            sendAnalogValue(i);
          }
        } 

      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(13, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}


void sendAnalogValue(int pinNum) {
  int pinValue = analogRead(myAPins[pinNum]);

  int val2 = pinValue/256;
  int val1 = pinValue - val2*256;

    const unsigned char pinCharArray[2] = { (char)val2, (char)val1 };
    mychars[pinNum].setValue(pinCharArray, 2);  // and update the heart rate measurement characteristic
}

bool haschangedAnalog(int pinNum) {
  
    int currentValue = analogRead(myAPins[pinNum]);
    int previousValue = oldAValues[pinNum];

    if (currentValue != previousValue) {      
      oldAValues[pinNum] = currentValue;
      return true;
    } else {
      return false;
    }

}


void sendDigitalValue(int pinNum) {
  if (digitalRead(myDPins[pinNum]) == LOW) {
    char lowValue = LOW;
    myDchars[pinNum].setValue(lowValue);
  } else {
    char highValue = HIGH;
    myDchars[pinNum].setValue(highValue);
  }

  
    // and update the heart rate measurement characteristic
}

bool haschangedDigital(int pinNum) {
  
    char currentValue = digitalRead(myDPins[pinNum]);
    char previousValue = oldDValues[pinNum];

    if (currentValue != previousValue) {      
      oldDValues[pinNum] = currentValue;
      return true;
    } else {
      return false;
    }

}



void listener0(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  0);
}

void listener1(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  1);
}

void listener2(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  2);
}

void listener3(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  3);
}

void listener4(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  4);
}

void listener5(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  5);
}

void listener6(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  6);
}

void listener7(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  7);
}

void listener8(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  8);
}

void listener9(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  9);
}

void listener10(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  10);
}

void listener11(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  11);
}

void listener12(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  12);
}
void listener13(BLECentral& central, BLECharacteristic& characteristic) {
  switchCharacteristicWritten(central, characteristic,  13);
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic, int pinNum) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  Serial.print(pinNum);  
  Serial.print(" - ");  
  Serial.print(characteristic.properties());  
  Serial.print(" - ");
  Serial.print(*characteristic.value());

  if (*characteristic.value() == 1) {
    Serial.println("LED on");
    digitalWrite(myDPins[pinNum], HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(myDPins[pinNum], LOW);
  }
}

