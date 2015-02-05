/*
UINumberElement
  This class allows the User to edit the value of an integer from the User Menu
 Created by: Justin Read
 11/19/2014
 */
#include <Arduino.h>
//#include "UIElement.h"
#include "UINumberElement.h"
/*
  Constructor for UINumberElement
  takes many items as arguments used to set the state of the datafields
*/
UINumberElement :: UINumberElement(boolean isSelectable,int row, void (*inputPrinter)(String,int,int, boolean), String inputText,int &inputNumber, int inputMin, int inputMax) //indicates that this class is derived from UIElement
{
  selectable = isSelectable;
  displayedText = inputText;
  numberStored = &inputNumber;
  minNumber = inputMin;
  maxNumber = inputMax;
  printToLCD = inputPrinter;
  rowOccupied = row;
}
/*
  updateLCD >> This method takes prints the displayedText and numberStored to the position(1,rowOccupied)
 It prints to 1 since 0 is reserved for the selection cursor. 
 True is sent because the line must be cleared prior to printing.
 */
void UINumberElement :: updateLCD()
{
  printToLCD(displayedText + *numberStored,1,rowOccupied,true);
}
/*
  circularIntAdjuster
  Takes a target integer by reference. Adds the adjustment to it, and makes sure it is within the bounds(inclusive)
  If a number exceeds either bound it is circularly transfered to the opposite bound.
*/
void UINumberElement :: circularIntAdjuster(int adjustment)
{
  *numberStored += adjustment;
  if (*numberStored < minNumber)
  {
    *numberStored = maxNumber;
  }
  if (*numberStored > maxNumber)
  {
    *numberStored = minNumber;
  } 
}
/*
    execute(overrides base class UIElement
    This method takes button input and executes the desired functionality of this method
    In this case it will increment or decrement a target integer
    UP and RIGHT increment
    DOWN and LEFT decrement
*/
int UINumberElement :: execute(int buttonPressed)
{
  int result = 0;
  switch(buttonPressed) 
  {
  case 0: // press up button
    circularIntAdjuster(1);
    break;
  case 1: //press right
    circularIntAdjuster( 1);
    break;
  case 2: // press up button
    circularIntAdjuster(-1);
    break;
  case 3: //press down
    circularIntAdjuster(-1);
    break;
  case 4:
    result = -1;
    break;
  }
  return result; 
}
