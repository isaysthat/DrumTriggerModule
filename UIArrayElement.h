/*
UIArrayElement
This class is derived from UIElement. It stores a String and when updateLCD is called will print that String to the rowOccupied.
This element should not be selectable as it does not explicitly override the execute method.
Created by: Justin Read
11/19/2014
*/
#ifndef UIARRAYELEMENT_H
#define UIARRAYELEMENT_H
#include <Arduino.h>
#include "UIElement.h"
#define numPads 6
class UIArrayElement : public UIElement //indicates that this class is derived from UIElement
{
public://allows any class in the project to manipulate it
  UIArrayElement(boolean isSelectable, 
                  int row, 
                  void (*inputPrinter)(String,int,int, boolean), 
                  String inputText,
                  int *inputArray[numPads], 
                  int &inputNumber, 
                  int inputMin, 
                  int inputMax, 
                  int inputStep);
  int stepStored;
  int execute(int buttonPressed);
  void updateLCD();
  int *arrayStored[numPads];
  int *numberStored;
  int minNumber;
  int maxNumber;
  String displayedText; 
  void circularIntAdjuster(int adjustment, int lower, int upper);
};
#endif
