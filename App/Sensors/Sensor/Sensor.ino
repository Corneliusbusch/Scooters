

class Sensor {
    
   public: 
    int pin;
    int baseVal = 0;
    int prevStates[];
    int threshold;
    int noReadings = 0;

  public:
    Sensor(int attachedTo, int prev, int thres) :
      pin = attachedTo;
      prevStates[] = realloc(baseVals, prev * sizeof(int));
      threshold = thres;
    }

    void setBaseVal(int newVal){
      baseVal = newVal;
    }

    bool isTriggeredValue(int newVal){
      
      // enqueue new value
      enqueuePrevStates(newVal);

      
      // check how many were triggered from prevStates
      int counter;
      int len = sizeof(prevStates) / sizeof(prevStates[0]);
      if(noReadings < len){
        counter = noReadings;
      }else{
        counter = len;
      }

      int trigg = 0;
      for(int i = 0; i < counter; i++){
        int diff = abs(baseVal - prevStates[i]);
        if(diff > threshold){
          trigg++;
        }
      }


      // if number triggered larger than not triggered, then accepted
      if(triggered > counter/2){
        return(true);
      } else{
        return(false);
      }
      
      
    }

    private: 
      void enqueuePrevStates(int value){
         
         int len = sizeof(prevStates) / sizeof(prevStates[0]);
         int last; 
         if(noReadings >= len){
            int discard = dequeuePrevStates();
            last = len-1;
         } else{
            last = noReadings;
         }
         prevVals[last] = value;
         noReadings++;
      }

      int dequeuePrevStates(){
         int len = sizeof(prevStates) / sizeof(prevStates[0]);
         int first = prevStates[0];
         for(int i = 1; i < len; i++){
          prevStates[i-1] = prevStates[i];
         }
         prevStates[len-1] = 0;
      }
};
