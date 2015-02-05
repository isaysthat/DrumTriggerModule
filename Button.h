
#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>// allows use of arduino functions
/*
  Button
This is the Header File for Class Button.
It defines the data Fields, Constructors and Methods used by this class.
The above are implemented in the accompanying .cpp file.
*/
class Button
{
public:
  byte pinNumber;
  unsigned long lastPushReading;
  Button();
  boolean isPushed(int deBounceTime);
};
#endif
