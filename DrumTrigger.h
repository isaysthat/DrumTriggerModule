/*
Drum Trigger Library
  This object holds data that a drum trigger needs to operate
Created by: Justin Read
10/20/2014
*/

#ifndef DRUMTRIGGER_H
#define DRUMTRIGGER_H
#include <Arduino.h>
class DrumTrigger
{
public://allows any class in the project to manipulate it
  DrumTrigger();
  byte pinNumber; //id which pin of arduino to read
  int lastReading; // what the last reading was
  unsigned long timeActivated;
  boolean activated;
  int currentMax;
  int noteValue; 
  int thresholdLower;
  int thresholdUpper;
  int minVelocity;
  int maxVelocity;
};
#endif
