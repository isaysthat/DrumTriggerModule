/*
UITextElement
 Derived from UIElement. This stores a String which can be displayed to the LCD.
 Created by: Justin Read
 11/13/2014
 */
#include <Arduino.h>
//#include "UIElement.h"
#include "UITextElement.h"
UITextElement :: UITextElement(boolean isSelectable, int row, void (*inputPrinter)(String,int,int,boolean), String inputText) //indicates that this class is derived from UIElement
{
  selectable = isSelectable;
  displayedText = inputText;
  printToLCD = inputPrinter;
  rowOccupied = row;
}
/*
  updateLCD >> This method takes prints the displayedText to the position(1,rowOccupied)
 It prints to 1 since 0 is reserved for the selection cursor. 
 True is sent because the line must be cleared prior to printing.
 */
void UITextElement :: updateLCD()
{
  printToLCD(displayedText, 1, rowOccupied, true);
}
/*
  This method doesn't do anything for the Text Element, but is here to override the virtual function.
 */
int UITextElement :: execute(int buttonState)
{
}


