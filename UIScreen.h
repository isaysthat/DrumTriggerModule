/*
UIScreen
  This library is the User Interface Screen class. This class is the main class of the user interface.
  This screen holds other screen elements and allows the user to select different pages, or
  different functions.
Created by: Justin Read
10/20/2014
*/
#ifndef UISCREEN_H
#define UISCREEN_H

#include <Arduino.h>
#include "UIElement.h"
#include "DrumTrigger.h"
#define numPads 6
class UIScreen
{
public://allows any class in the project to manipulate it
  UIScreen();
  DrumTrigger *pad;
  int *noteValues[numPads];
  int *thresholdLower[numPads];
  int *thresholdUpper[numPads];
  int *velocityLower[numPads];
  int *velocityUpper[numPads];
  int *lastReading[numPads];
  //int *midiChannel;
  void (*inputPrinter)(String,int,int,boolean);
  
  void init(void (*argPrinter)(String,int,int,boolean),DrumTrigger inPad[numPads], int numTriggers,int &hiHatNoteValue);
  void setElement(UIElement &newElement, int screen, int row);
  
  void printRow(String text, int row);
  void lcdSetCursor(int col, int row);
  void printSelectionCursor();
  
  int numberOfScreens;
  UIElement *elementPtr[4][4]; //pointer to the particular UI element in question
  int screenSelected;
  int elementSelected; // which row is selected on this screen
  int padSelected;
  int presetSelected;
  boolean locked;
  void buttonInput(int buttonPressed);
  /*the & in the following line means this is a byRef operation.
    this function will directly access and change the target variable
  */
  void update();
  void circularIntAdjuster(int& target, int adjustment, int lower, int upper);
};

#endif
