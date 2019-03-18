#include <Sensor.h>
#include <SoftwareSerial.h>
    
const int sensorPins[] = {0, 1, 2};    // Analog pins for sensors
int numOfPins = sizeof(sensorPins) / sizeof(sensorPins[0]);
const int threshold = 100;
const int prevValuesSaved = 7;

int * triggered = (int*) realloc(triggered, numOfPins * sizeof(int));

bool readingOn = true;

Sensor* sensors = realloc(sensors, numOfPins * sizeof(Sensor));

const int buttonCalibration = 8;
// button variables
unsigned long lastDebounceCalibration = 0;
unsigned long debounceDelay = 150;

void setUpSensors(){
  //Sensor::Sensor(int attachedTo, int prev, int thres)
  for(int i = 0; i < numOfPins; i++){
    Sensor sen = Sensor(sensorPins[i], prevValuesSaved, threshold);
    sensors[i]= sen; 
  }
}


void calibrate(){
  //Serial.println("Start Calibration.");
  for(int i = 0; i < numOfPins; i++){
    int val = analogRead(sensorPins[i]);
    sensors[i].baseVal = val;
    //Serial.println(sensors[i].baseVal);
    //sensors[i].printSensor();
  }
  //printArray(baseVals, numOfPins);
  //Serial.println("Calibration Done");
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

    Sensor sen = sensors[i];
    
    int val = analogRead(sen.pin);
    int diff = abs(sen.baseVal - val);


    //Serial.println("sensor: " + String(i) + " has value " + String(val));
    
    //Serial.println(diff);
    if(sen.isTriggeredValue(val)){
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
  for(int i= 0; i < size; i++){
    if(arr[i] != 0){
      Serial.write(i);  //Send this number as a single byte
    }
  }
}
