#include "Button.h"
#include <Arduino.h>

/*

 The button class stores the pin associated with the button
 Also store the lastPushReading as an unsigned long. 
 this value is the time (in milliseconds) of the last press
 In tandem with the Debounce time this keeps buttons from double clicking
 */


/*
  Constructor
 Does nothing since these objects don't need initializing.
 */
Button::Button()
{
}

/*
  isPushed()
 This method will return true if the button has been pressed.
 It also makes sure it has not been pressed recently (within the deBounceTime).
 DeBouncing keeps buttons from being pressed multiple times when the user only intended one press.
 */
boolean Button::isPushed(int deBounceTime)
{
  if ((millis() > lastPushReading + deBounceTime) &&  digitalRead(pinNumber) == LOW)
  {
    lastPushReading = millis();
    return true;
  }
  else{
    return false;
  }
}


