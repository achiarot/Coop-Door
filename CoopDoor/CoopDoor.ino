/*COOP DOOR CONTROLLER
 * This sketch controls the door opening and closing time for the coop
 */

#define INPUT_PULLUP 0x2
/*INPUTS
 * 1 - Light Sensor
 * 2 - Manual Switch
 * 3 - Door Closed Endstop
 * 4 - Door Open Endstop
 */
 #define lightSense 2 //Use pin 2 = analog 1 so that it can be used as an analog input later
 #define swMan 3
 #define swClosed 4
 #define swOpen 5

/* OUTPUTS
 * 1 - Output for relay 1  
 * 2 - Output for relay 2
 */
 #define relay1 0
 #define relay2 1

 int opening;
 int closing;
 long long timeOut;
 int lightChange;
 int lightDetected;
 long long lightTimer;

void setup() {
  pinMode(lightSense, INPUT);
  pinMode(swMan, INPUT_PULLUP);
  pinMode(swClosed, INPUT_PULLUP);
  pinMode(swOpen, INPUT_PULLUP);

  //relays set to high are the NC state
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
}

void loop() {
  int currentState = state();
  
  if(!digitalRead(swMan) || lightChange){
        switch (currentState){
          case 0:  //it is open, now close it
            moveDoor(0);
            break;
          case 1:
            moveDoor(1);
            break;
          case 2:
            if(closing){
              moveDoor(0);
            }
            else if(opening){
              moveDoor(1);
            }
            break;
       }
       //software debounce the pushbutton
       delay(50);
       lightChange = 0;
       lightDetected = 0;
  }
  else if(closing || opening){
    if((closing && !digitalRead(swClosed)) || (opening && !digitalRead(swOpen)) || (millis() - timeOut > 10000)){
      stopDoor();
      //software debounce the switch
      delay(50);
    }
  }
  else if((!digitalRead(lightSense) != currentState) && (currentState != 2)){
    if(!lightDetected){
      lightTimer = millis();
    }
    lightDetected = 1;
    //10 minute time delay = 360000
    if(millis() - lightTimer > 360000){
      lightChange = 1;
    }
  }
}

//open or close the door. 0 = close, 1 = open
void moveDoor(int doorMovement){
  stopDoor();
  if(doorMovement == 0){
    digitalWrite(relay1, LOW);
    closing = 1;
  }
  else{
    digitalWrite(relay2, LOW);
    opening = 1;
  }
  timeOut = millis();
}

void stopDoor(){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  closing = 0;
  opening = 0;
}

//Current state of the door: 0 = Closed, 1 = Open
int state(){
  int result;

  if(!digitalRead(swClosed)){
    result = 0;
  }
  else if(!digitalRead(swOpen)){
    result = 1;
  } 
  else{
    result = 2;
  }
  return result;
}
















