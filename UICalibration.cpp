/*
UICalibrationElement
 Created by: Justin Read
 11/24/2014
 */
#include <Arduino.h>
#include "UICalibrationElement.h"
#define numPads 6
/*
  Constructor for UICalibrationElement
 takes many items as arguments used to set the state of the datafields
 */
UICalibrationElement :: UICalibrationElement(boolean isSelectable,
int row, 
void (*inputPrinter)(String,int,int, boolean),
int *inputArray[numPads], 
int *minArray[numPads],
int *maxArray[numPads],
int &inputNumber) 
{
  selectable = isSelectable;
  for (int i = 0; i < numPads; i++) //copy array values
  {
    readingStored[i] = inputArray[i];
    lowerStored[i] = minArray[i];
    upperStored[i] = maxArray[i];
  }
  numberStored = &inputNumber;
  printToLCD = inputPrinter;
  rowOccupied = row;
  state =0; //represents which state of calibration we are in
  lowerThreshold =0;
  upperThreshold =0;
  noiseThreshold =0;
}
/*
  updateLCD >> This method takes prints the displayedText and numberStored to the position(1,rowOccupied)
 It prints to 1 since 0 is reserved for the selection cursor. 
 True is sent because the line must be cleared prior to printing.
 */
void UICalibrationElement :: updateLCD()
{
  switch(state)
  {
  case 0:
    printToLCD("1.Hit OK" + String(noiseThreshold),1,rowOccupied,true);
    break; 
  case 1:
    printToLCD("2.Tap Soft" + String(lowerThreshold),1,rowOccupied,true);
    break; 
  case 2:
    printToLCD("3.Tap Hard" + String(upperThreshold),1,rowOccupied,true);
    break; 
  case 3:
    if (lowerThreshold < upperThreshold)
    {
      printToLCD("Done" ,1,rowOccupied,true);
    }
    else
    {
      printToLCD("Error Retry" ,1,rowOccupied,true);
    }
    break; 
  }
}
/*
    execute(overrides base class UIElement
 This method takes button input and executes the desired functionality of this method
 In this case it will increment or decrement a target integer
 UP/Down/Left/Right do nothing
 OK will make the state increase
 */
int UICalibrationElement :: execute(int buttonPressed)
{
  int result = 0;
  switch(state)
  {
  case 0:
    if (noiseThreshold < *readingStored[*numberStored])
    {
      noiseThreshold = *readingStored[*numberStored];
      updateLCD();
    }
    break; 
  case 1:
    if (lowerThreshold < *readingStored[*numberStored])
    {
      lowerThreshold = *readingStored[*numberStored];
      updateLCD();
    }
    break; 
  case 2:
    if (upperThreshold < *readingStored[*numberStored])
    {
      upperThreshold = *readingStored[*numberStored];
      updateLCD();
    }
    break; 
  }
  if (buttonPressed == 4) //if you hit ok here.
  {
    if (state ==3)
    {
      if (lowerThreshold < upperThreshold)
      {//calibration is valid
        *lowerStored[*numberStored] = ((lowerThreshold+noiseThreshold) / 2);
        *upperStored[*numberStored] = upperThreshold;
      }

      //if calibraton is valid or not, we reset the stored variables here so next time they are initialized
      lowerThreshold =0;
      upperThreshold =0;
      noiseThreshold =0;
      state =0;
      result = -1;
    }
    else
    {
      state++;
    } 
  }
  //here the other buttons are irrelevant, so we only check if buttonPressed == 4
  return result; 
}
