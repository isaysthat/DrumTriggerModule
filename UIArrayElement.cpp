/*
UIArrayElement
 Created by: Justin Read
 11/23/2014
 */
#include <Arduino.h>
//#include "UIElement.h"
#include "UIArrayElement.h"
#define numPads 6
/*
  Constructor for UINumberElement
 takes many items as arguments used to set the state of the datafields
 */
UIArrayElement :: UIArrayElement(boolean isSelectable,
int row, 
void (*inputPrinter)(String,int,int, boolean), 
String inputText,
int *inputArray[numPads], 
int &inputNumber, 
int inputMin, 
int inputMax,
int inputStep) //indicates that this class is derived from UIElement
{
  stepStored = inputStep;
  selectable = isSelectable;
  displayedText = inputText;
  for (int i = 0; i < numPads; i++)
  {
    arrayStored[i] = inputArray[i];
  }
  numberStored = &inputNumber;
  minNumber = inputMin;
  maxNumber = inputMax;
  printToLCD = inputPrinter;
  rowOccupied = row;
}
/*
  updateLCD()
 defines how this Element will produce output.
 */
void UIArrayElement :: updateLCD()
{
  printToLCD(displayedText + *arrayStored[*numberStored],1,rowOccupied,true);
}
/*
  circularIntAdjuster
 Takes a target integer by reference. Adds the adjustment to it, and makes sure it is within the bounds(inclusive)
 If a number exceeds either bound it is circularly transfered to the opposite bound.
 */
void UIArrayElement :: circularIntAdjuster(int adjustment, int lower, int upper)
{
  //asterisk(*) denotes dereference. Read the value of the variable pointed to, not the address
  *arrayStored[*numberStored] += adjustment;
  if (*arrayStored[*numberStored] < lower)
  {
    *arrayStored[*numberStored] = upper;
  }
  if (*arrayStored[*numberStored] > upper)
  {
    *arrayStored[*numberStored] = lower;
  } 
}
/*
    execute(overrides base class UIElement
 This method takes button input and executes the desired functionality of this method
 In this case it will increment or decrement a target integer
 UP and RIGHT increment
 DOWN and LEFT decrement
 */
int UIArrayElement :: execute(int buttonPressed)
{
  int result = 0;
  switch(buttonPressed) 
  {
  case 0: // press up button
    circularIntAdjuster(stepStored,minNumber,maxNumber);
    break;
  case 1: //press right
    circularIntAdjuster(stepStored, minNumber, maxNumber);
    break;
  case 2: // press up button
    circularIntAdjuster( -stepStored, minNumber, maxNumber);
    break;
  case 3: //press down
    circularIntAdjuster(-stepStored, minNumber, maxNumber);
    break;
  case 4:
    result = -1;
    break;
  }
  return result; 
}



