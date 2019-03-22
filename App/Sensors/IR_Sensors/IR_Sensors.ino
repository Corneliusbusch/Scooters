#include <Sensor.h>
#include <SoftwareSerial.h>
    
const int sensorPins[] = {0, 1, 2};    // Analog pins for sensors
int numOfPins = sizeof(sensorPins) / sizeof(sensorPins[0]);
const int threshold = 100;
const int prevValuesSaved = 7;

int *triggered = new int[numOfPins];

bool readingOn = true;

Sensor **sensors = new Sensor*[numOfPins];

const int buttonCalibration = 8;
// button variables
unsigned long lastDebounceCalibration = 0;
unsigned long debounceDelay = 150;

void setUpSensors(){
  //Sensor::Sensor(int attachedTo, int prev, int thres)
  for(int i = 0; i < numOfPins; i++){
    sensors[i] = new Sensor(sensorPins[i], prevValuesSaved, threshold);
  }
}


void calibrate(){
  Serial.println("Start Calibration.");
  for(int i = 0; i < numOfPins; i++){
    int val = analogRead(sensorPins[i]);
    sensors[i]->baseVal = val;
    Serial.println(sensors[i]->baseVal);
    //sensors[i].printSensor();
  }
  //printArray(baseVals, numOfPins);
  Serial.println("Calibration Done");
}

void setup()
{
  Serial.begin(9600);               // starts the serial monitor

  
  //Set up sensors
  setUpSensors();
  
  pinMode(buttonCalibration, INPUT);
  
  calibrate();
  
}

bool isButtonPressed(int button){
  if((millis() -lastDebounceCalibration)>debounceDelay){
     int buttonState = digitalRead(button);
     if(buttonState == HIGH){
      lastDebounceCalibration = millis();
      return(true);
     }      
  }
  return(false);
}

bool isAcceptedValue(int current, int pointer){
  return(true);
}

bool readSensors(){
  //Serial.println("Read Sensors");
  bool resend = false;
  for(int i = 0; i < numOfPins; i++){
    
    int val = analogRead(sensors[i]->pin);
    int diff = abs(sensors[i]->baseVal - val);


    //Serial.println("sensor: " + String(i) + " has value " + String(val));
    
    //Serial.println(diff);
    if(sen.isTriggeredValue(val)){
      //Serial.println("sensor: " + String(i) + ". Original: " + String(baseVals[i]) + " now: " + String(val) + " Difference: " + String(diff));
      Serial.println(diff);
      // triggered
      if (triggered[i] == 0){
        triggered[i] = 1;
        resend = true;
      }
    } 
    
    else{
      
      // not triggered
      if(triggered[i] != 0){
        triggered[i] = 0;
        resend = true;
      }
    }
    
  }
  return(resend);
  
}
 
void loop()
{
  // check for reading button
  
  // check for calibration button
  if(isButtonPressed(buttonCalibration)){
    calibrate();
  }

  // read sensor data
  if (readingOn){
    if(readSensors()){
      // change found
        printArray(triggered, numOfPins);
    }
  }

  delay(500);                    // wait for this much time before printing next value
}

void printArray(int arr[], int size){
  Serial.print("[");
  for(int i= 0; i < size; i++){
        Serial.print(arr[i]);
        if(i == size-1){
          Serial.println("]");
        }else{
          
        Serial.print(", ");
        }
        
  }
}
