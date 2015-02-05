/*
UITextElement
 This class is derived from UIElement. It stores a String and when updateLCD is called will print that String to the rowOccupied.
 This element should not be selectable as it does not explicitly override the execute method.
 Created by: Justin Read
 11/2/2014
 */
#ifndef UITEXTELEMENT_H
#define UITEXTELEMENT_H
#include <Arduino.h>
#include "UIElement.h"
class UITextElement : 
public UIElement //indicates that this class is derived from UIElement
{
public://allows any class in the project to manipulate it
  UITextElement(boolean isSelectable, int row, void (*inputPrinter)(String, int, int, boolean), String inputText);
  int execute(int buttonState);
  void updateLCD();
  String displayedText;
};
#endif

