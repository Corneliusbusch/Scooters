#include <Sensor.h>
#include <SoftwareSerial.h>
    
const int sensorPins[] = { 0, 1, 2};    // Analog pins for sensors
int numOfPins = sizeof(sensorPins) / sizeof(sensorPins[0]);
const int threshold = 150;

//int* baseVals = 0;         // variable to store the values from sensor(initially zero)
int* baseVals = (int*) realloc(baseVals, numOfPins * sizeof(int));
//int* triggered = 0;
int * triggered = (int*) realloc(triggered, numOfPins * sizeof(int));
const int prevValuesSaved = 3;

//TODO add a filter, if triggered at least twice, admit as trigger
//TODO disregard minor outbreaks
//Count how many are going over the threshold in the last 400ms. If average is over, then trigger.
int filterCount[] = {0,0,0};
bool readingOn = true;

//int preVals[][]



const int buttonCalibration = 8;
// button variables
unsigned long lastDebounceCalibration = 0;
unsigned long debounceDelay = 150;



void calibrate(){
  Serial.println("Start Calibration.");
  for(int i = 0; i < numOfPins; i++){
    baseVals[i] = analogRead(sensorPins[i]);
    triggered[i] = 0;
  }
  printArray(baseVals, numOfPins);
  Serial.println("Calibration Done");
}

void setup()
{
  Serial.begin(9600);               // starts the serial monitor
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
    int val = analogRead(sensorPins[i]);
    int diff = abs(baseVals[i] - val);
    //Serial.println("sensor: " + String(i) + " has value " + String(val));
    
    //Serial.println(diff);
    if(diff >= threshold){
      //Serial.println("sensor: " + String(i) + ". Original: " + String(baseVals[i]) + " now: " + String(val) + " Difference: " + String(diff));
      
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

  delay(100);                    // wait for this much time before printing next value
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
