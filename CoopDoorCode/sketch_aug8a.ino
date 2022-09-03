/*

Project: Auto chicken coop door that opens and closes based on the light level outside (whether its day or night out)
By: George Waltz
Date: 7.26.2022

Features: - 2 Limit switches to indicate if the door is in the up position or down position
          - LED light to indicate the transitory state of the door (red = down; green = up; yellow = moving)
          - Button for userto manually open or close the door
          - motor to open and close the door

*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int PWMA = 3;  // PWM control (speed) for motor A
int AIN1 = 12; // Direction control for motor A

int lightSensor = A0;  //Light Sensor pin A0
int bottomPin = 8;     //Bottom LimitSwitch pin 8
int topPin = 10;       //Top Limit Switch pin 10
int readButton = 7;   //Button pin 12
int potentiometer = A1;

int redPin = 6;        //LED red pin 6
int greenPin = 4;      //LED green pin 4
int bluePin = 2;       //LED blue pin 3

int lightVal = 0;
int flagDirection = 1;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void flagSwitch() {

  if (flagDirection == 1) {

    flagDirection = 0;
  }
  else if (flagDirection == 0) {

    flagDirection = 1;
  }
}

void setColor(int red, int green, int blue) { //LED set color function

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void startUp() { //LED powering on function

  setColor(0, 255, 0);
  delay(800);
  setColor(255, 255, 0);
  delay(800);
  setColor(255, 0, 0);
  delay(800);
}

void doorUp() {

  delay(10000); //wait two minutes 
  if (digitalRead(bottomPin) == LOW && analogRead(lightSensor) > lightVal) { //double checks to see if conditions are met 

    // delay(600000); //wait ten minutes
    while (digitalRead(topPin) != LOW) { // move door up until top pin is triggered

      analogWrite(PWMA, 255);  // turn on motor and set speed to 255
      digitalWrite(AIN1, flagDirection);
      setColor(255, 255, 0);
    }

    flagSwitch();
    analogWrite(PWMA, 0); //motor off--> door is up
    setColor(0, 255, 0);
  }
}

void doorDown() {

  delay(10000); //wait two minutes 
  if (digitalRead(topPin) == LOW && analogRead(lightSensor) < lightVal) { //double checks to see if the conditions are met
    // delay(600000); //delay ten minutes
    while (digitalRead(bottomPin) != LOW) { 

      analogWrite(PWMA, 255); // move door down until bottom pin is triggered
      digitalWrite(AIN1, flagDirection);
      setColor(255, 255, 0);
    }

    flagSwitch();
    analogWrite(PWMA, 0); //motor off--> door is up
    setColor(255, 0, 0);
  }
}

void manualControl() {  //manual Button control when user wants to manually open the coop

  if (digitalRead(bottomPin) == LOW) {  //checks to see if bottom pin is triggered ~~~~~~~~~~~~~~ makes door go up ~~~~~~~~~~~~~~~~

    while (digitalRead(topPin) != LOW) { // move door up until top pin is triggered

      analogWrite(PWMA, 255);  
      digitalWrite(AIN1, flagDirection);
      setColor(255, 255, 0);
    }

    flagSwitch();
    analogWrite(PWMA, 0); //motor off--> door is up
    setColor(0, 255, 0);
    specialCondition(); //goes into special condition function

  } else if (digitalRead(topPin) == LOW) {  // checks to see if top pin is triggered ~~~~~~~~~~~~~~ makes door go down ~~~~~~~~~~~~~~~~

    while (digitalRead(bottomPin) != LOW) { // move door down until bottom pin is triggered

      analogWrite(PWMA, 255); 
      digitalWrite(AIN1, flagDirection);
      setColor(255, 255, 0);
    }

    flagSwitch();
    analogWrite(PWMA, 0);  //motor off--> door is up
    setColor(255, 0, 0);
    specialCondition(); //goes into special condition function

  } else if (digitalRead(topPin) != LOW && digitalRead(bottomPin) != LOW) {  // if somehow the door is touching neither pin the door will go up to reset orientation

    while (digitalRead(topPin) != LOW) { // move door up until top pin is triggered

      setColor(255, 255, 0);
      analogWrite(PWMA, 255);
      digitalWrite(PWMA, 1);
    }

    flagDirection = 0;
    analogWrite(PWMA, 0); //motor off--> door is up
    setColor(0, 255, 0);
  }
}

void specialCondition() { //for when door is manually closed and it is light out or when door is manually opened and it is dark out

  unsigned long start_time = millis(); //set millis to use for timer
  while (1) { //while loop so arduino does not imidiately go back into the loop function
    
    if (digitalRead(readButton) == LOW) { //if user pushes button again while loop will break and code will go back into the loop function
        
        break;
    } 
     else if (millis() - start_time >= 600000) { //if user forgets to push button --> after 10 min the arduino will break the while lopp and go back into the loop function

      break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);

  Serial.begin(9600); 
  pinMode(redPin, OUTPUT);  //LED pin setup
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(bottomPin, INPUT_PULLUP); //sensor pin setup
  pinMode(topPin, INPUT_PULLUP);
  pinMode(readButton, INPUT_PULLUP);
  pinMode(potentiometer, INPUT);
  
  startUp();
  if (digitalRead(topPin) != LOW) { //checks to see if door is already up 

    while (digitalRead(topPin) != LOW) { //runs motor in the up direction untill the top pin is triggered

      Serial.println("Started");
      digitalWrite(AIN1, flagDirection);
      analogWrite(PWMA, 255);
      setColor(255, 255, 0);
    }
  }

  flagSwitch();
  analogWrite(PWMA, 0);
  setColor(0, 255, 0);
}

void loop() {

  pinMode(readButton, INPUT_PULLUP);  //Button pin setup ~~~for some reason I couldnt get it to work by just setting it up in the setup function~~~
  Serial.println(analogRead(lightSensor));
  delay(1000);
  lightVal = analogRead(potentiometer)/4;
  Serial.println(lightVal);
  if (digitalRead(topPin) == LOW && analogRead(lightSensor) < lightVal) { //parameters for door to go down --> dark out and door is up/top switch is on

    doorDown();
  } else if (digitalRead(bottomPin) == LOW && analogRead(lightSensor) > lightVal) { // parameters for door to go up --> light out and door is down/bottom switch is on
     
    doorUp();
  } else if (digitalRead(readButton) == LOW) { // manual control if user wants to open and close the door (this bypasses the lightsensor and does not take into account light level)

    manualControl();
  }
}
