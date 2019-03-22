#include <Sensor.h>
#include <SoftwareSerial.h>
    
const int sensorPins[] = {0, 1, 2};    // Analog pins for sensors
const int thresholds[] = {130, 130, 130};    // Analog pins for sensors
int numOfPins = sizeof(sensorPins) / sizeof(sensorPins[0]);
const int threshold = 120;
const int prevValuesSaved = 5;

int * triggered = (int*) realloc(triggered, numOfPins * sizeof(int));

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
    sensors[i]->baseVal = analogRead(sensorPins[i]);
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

void readAndSendSmoothReadings(){
  int *vals = new int[numOfPins];
  
  for(int i = 0; i < numOfPins; i++){
    int val = analogRead(sensors[i]->pin);

    sensors[i]->addReading(val);

    int smoothed = sensors[i]->getSmoothReading();
    sendReading(smoothed);

    vals[i] = smoothed;
  }

  printArray(vals, numOfPins);
}


 

bool readSensors(){
  //Serial.println("Read Sensors");
  bool resend = false;
  for(int i = 0; i < numOfPins; i++){
    
    int val = analogRead(sensors[i]->pin);
    int diff = val - sensors[i]->baseVal;


    //Serial.println("sensor: " + String(i) + " has value " + String(val));
    
    //Serial.println(diff);
    if(sensors[i]->isTriggeredValue(val)){
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
    /*if(readSensors()){
      // change found
        printArray(triggered, numOfPins);
    }*/

    readAndSendSmoothReadings();
  }

  delay(300);                    // wait for this much time before printing next value
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

void sendArray(int arr[], int size){
    for(int i= 0; i < size; i++){
    if(arr[i] != 0){
      Serial.write(i);  //Send this number as a single byte
    }
  }
}

void sendReading(int val){
  //TODO byteshifts
  Serial.write(val);
}
