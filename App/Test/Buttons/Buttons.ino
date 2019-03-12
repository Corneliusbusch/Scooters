    #include <SoftwareSerial.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

#define CHIP_SELECT 10                         // For an Uno it's 10

SoftwareSerial SoftSerial(8, 7);
File logfile;

// these constants describe the pins. They won't change:
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int buttonSound = 8;     // the number of the pushbutton pin
const int buttonVideo = 9;     // the number of the pushbutton pin

const int ledSound =  6;      // the number of the LED pin
const int ledVideo = 7; 

// button variables
unsigned long lastDebounceSound = 0;
unsigned long lastDebounceVideo = 0;
unsigned long debounceDelay = 150;



void setup() {
  // initialize the serial communications
  Serial.begin(9600);

  // LED pin
  pinMode(ledSound, OUTPUT);
  pinMode(ledVideo, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonSound, INPUT);
  pinMode(buttonVideo, INPUT);
}

unsigned long handleButton(int button, int led){

      // read Button
    int buttonState = digitalRead(button);

    // While button is high, play
    if (buttonState == HIGH) {
        digitalWrite(led, HIGH);
        Serial.println("pressed");
      }
      else{
        digitalWrite(led, LOW);
        //Serial.println("un-pressed");
      }
    return millis();
  }

void loop() {  
  
  if((millis() - lastDebounceSound) > debounceDelay){

    lastDebounceSound = handleButton(buttonSound, ledSound);
  }
  
  if((millis() - lastDebounceVideo) > debounceDelay){

    lastDebounceVideo = handleButton(buttonVideo, ledVideo);
    
  }


  // delay before next reading:
  delay(100);

}
