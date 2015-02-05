/*
UICalibrationElement
 This class is derived from UIElement. 
 This element stores a reference array to the different triggers lastReading. It allows this element to read the input from the user
 and from that input set the thresholds for each pad to their optimal settings.
 Created by: Justin Read
 11/24/2014
 */

#ifndef UICALIBRATIONELEMENT_H
#define UICALIBRATIONELEMENT_H
#include <Arduino.h>
#include "UIElement.h"
#define numPads 6
class UICalibrationElement : 
public UIElement //indicates that this class is derived from UIElement
{
public://allows any class in the project to manipulate it
  UICalibrationElement(boolean isSelectable, int row, void (*inputPrinter)(String,int,int, boolean),int *inputArray[numPads], int *minArray[numPads], int *maxArray[numPads], int &inputNumber);
  int state;
  int execute(int buttonPressed);
  void updateLCD();
  void updateNumberStored(int &newNumber);
  int *readingStored[numPads];
  int *lowerStored[numPads];
  int *upperStored[numPads];
  int noiseThreshold;
  int lowerThreshold;
  int upperThreshold;
  int *numberStored;
};
#endif

