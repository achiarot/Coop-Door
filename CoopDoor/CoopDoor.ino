/*COOP DOOR CONTROLLER
 * This sketch controls the door opening and closing time for the coop
 */

/*INPUTS
 * 1 - Light Sensor
 * 2 - Manual Switch
 * 3 - Door Closed Endstop
 * 4 - Door Open Endstop
 */
 #define lightSense 8
 #define swMan 4
 #define swClosed 5
 #define swOpen 6

/* OUTPUTS
 * 1 - Output for relay 1  
 * 2 - Output for relay 2
 */
 #define relay1 2
 #define relay2 3

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

  //debug code
  Serial.begin(9600);
}

void loop() {
  int currentState = state();
  
  if(!digitalRead(swMan) || lightChange){

    //debug
    Serial.println("Change request detected");
        
        switch (currentState){
          case 0:  //it is closed, now open it
            moveDoor(1);
            break;
          case 1:
            moveDoor(0);
            break;
          case 2:
            if(closing){
              moveDoor(1);
            }
            else if(opening){
              moveDoor(0);
            }
            else{
              moveDoor(1);
            }
            break;
       }
       //software debounce the pushbutton
       delay(100);
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

    //debug code
    Serial.println("door is closing");
  }
  else{
    digitalWrite(relay2, LOW);
    opening = 1;

    //debug code
    Serial.println("door is opening");
  }
  timeOut = millis();
}

void stopDoor(){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  closing = 0;
  opening = 0;

  //debug code
  Serial.println("stopping the door");
}

//Current state of the door: 0 = Closed, 1 = Open
int state(){
  int result;

  if(!digitalRead(swClosed)){
    result = 0;

    //debug code
    Serial.println("door is closed");
  }
  else if(!digitalRead(swOpen)){
    result = 1;

    //debug code
    Serial.println("door is open");
  } 
  else{
    result = 2;

    //debug code
    Serial.println("door is between");
  }
  return result;
}
















