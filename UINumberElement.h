/*
UINumberElement
This class is derived from UIElement. It stores a String and when updateLCD is called will print that String to the rowOccupied.
Following the String is the number associated with the object.
Created by: Justin Read
11/19/2014
*/
#ifndef UINUMBERELEMENT_H
#define UINUMBERELEMENT_H
#include <Arduino.h>
#include "UIElement.h"
class UINumberElement : public UIElement //indicates that this class is derived from UIElement
{
public://allows any class in the project to manipulate it
  UINumberElement(boolean isSelectable, int row, void (*inputPrinter)(String,int,int, boolean), String inputText,int &inputNumber, int inputMin, int inputMax);
  int execute(int buttonPressed);
  void updateLCD();
  int *numberStored;
  int minNumber;
  int maxNumber;
  String displayedText; 
  void circularIntAdjuster(int adjustment);
};
#endif
