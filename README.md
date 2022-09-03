# Coop_Door_2022
Purpose:
  Automate the chicken coop door to make taking care of the chickens easier. 

Function: 
  Opens and closes the door to the chicken coop based on the exterior light level. At night the door will close when the chickens are inside. 
  When the light level is high enough the door will open to let the chickens out for the day.
  
Features:

  -Light sensor to open and close the door in the morning and at night
  
  -Solar panel and battery to power the arduino and motor
  
  -Button for user to manually close and open the door--door will automatically return to its original position after 10 min
  
  -RGB light to indicate what state the door is in (open, closed, moving)
  
  
Parts:

  -10W solar panel
  
  -Charge controller
  
  -12V battery
  
  -Arduino Uno
  
  -Motor Shield Controller
  
  -12V motor
  
  
 Sensors:
  -RGB LED - changes color depending on the transitory state of the door. (Green when door is open, Yellow when door is moving, Red when door is closed)
  
  -Photoresistor - detects light level
  
  -Potentiometer - used to adjust the light value the door will open/close at
  
  -2 Limit Switches - detect when the door is all the way up or all the way down
  
  -Button - manual switch for user to close the door during the day oor open it at night
  
 

Link to a youtube video demonstrating it working:
  https://youtu.be/3B1LDU-t6_Y
