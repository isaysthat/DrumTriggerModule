
/*
 The button class stores the pin associated with the button
 Also store the lastPushReading as an unsigned long. 
 this value is the time (in millis) of the last press
 In tandem with the Debounce time this keeps buttons from double clicking
 */
#include "UIScreen.h"
#include <Arduino.h>
#include "UITextElement.h"
#include "UINumberElement.h"
#include "UIArrayElement.h"
#include "UICalibrationElement.h"
#include "UISavePresetElement.h"
#include "UILoadPresetElement.h"
//#include "DrumTrigger.h"
#define numPads 6

/*
  Constructor is empty since the initialization requires the use of other libraries.
  Since we cannot be sure that those other libraries are already compiled we cannot
  initialize them in the constructor.
*/
UIScreen::UIScreen()
{
}

/*
  initializes the data fiels of the User Menu
*/
void UIScreen :: init(void (*argPrinter)(String,int,int,boolean), DrumTrigger inPad[numPads], int numTriggers, int &hiHatNoteValue)
{
  numberOfScreens = 4;
  inputPrinter = argPrinter;
  pad = inPad;
  for (int i = 0; i < numTriggers; i++)
  {
     noteValues[i] = &pad[i].noteValue; 
     thresholdLower[i] = &pad[i].thresholdLower;
     thresholdUpper[i] = &pad[i].thresholdUpper;
     velocityLower[i] = &pad[i].minVelocity;
     velocityUpper[i] = &pad[i].maxVelocity;
     lastReading[i] = &pad[i].lastReading;
  }
  /*
    The Element pointers point to different elements which populate the entire User Menu
  */
  elementPtr[0][0] = new UINumberElement(true, 0, inputPrinter, "Pad =", padSelected,0,5);
  elementPtr[0][1] = new UIArrayElement(true, 1, inputPrinter, "Note =", noteValues, padSelected, 24,99,1);
  elementPtr[0][2] = new UITextElement(false, 2, inputPrinter, "HiHat Pedal");
  elementPtr[0][3] = new UINumberElement(true, 3, inputPrinter, "Pressed Note=", hiHatNoteValue,24,99);

  elementPtr[1][0] = new UIArrayElement(true, 0, inputPrinter, "Min Thresh=", thresholdLower, padSelected, 5,1020,5);
  elementPtr[1][1] = new UIArrayElement(true, 1, inputPrinter, "Max Thresh=", thresholdUpper, padSelected, 5,1020,5);
  elementPtr[1][2] = new UIArrayElement(true, 2, inputPrinter, "Lower Vel =", velocityLower, padSelected, 1,127,1);
  elementPtr[1][3] = new UIArrayElement(true, 3, inputPrinter, "Upper Vel =", velocityUpper, padSelected, 1,127,1);
  
  elementPtr[2][0] = elementPtr[0][0];//just happens to be the same thing
  elementPtr[2][1] = new UITextElement(false,1,inputPrinter,"Calibration");
  elementPtr[2][2] = new UITextElement(false,2,inputPrinter,"Start Below");
  elementPtr[2][3] = new UICalibrationElement(true,3,inputPrinter,lastReading, thresholdLower, thresholdUpper,padSelected);
    
  elementPtr[3][0] = new UITextElement(false,0,inputPrinter,"Presets");
  elementPtr[3][1] = new UINumberElement(true, 1, inputPrinter, "Preset = ", presetSelected,0,9);
  elementPtr[3][2] = new UISavePresetElement(true, 2, inputPrinter, "Save", presetSelected, hiHatNoteValue, pad);
  elementPtr[3][3] = new UILoadPresetElement(true, 3, inputPrinter, "Load", presetSelected, hiHatNoteValue, pad);
}

/*
  Takes the button signal and depending on the state of the User Menu performs different actions.
*/
void UIScreen :: buttonInput(int buttonPressed)
{
  /*
   The button pressed is an integer corresponding with the following inputs
   Up = 0, right = 1, down = 2, left = 3, OK = 4
   */
  if (!locked)//while the screen is browsing elements, allows selecting different screens/elements
  {
    switch(buttonPressed) 
    {
    case 0: // press up button
      circularIntAdjuster(elementSelected,-1,0,3);
      break;
    case 1: 
      circularIntAdjuster(screenSelected, 1, 0,numberOfScreens-1);
      break;
    case 2: // press up button
      circularIntAdjuster(elementSelected,1,0,3);
      break;
    case 3: 
      circularIntAdjuster(screenSelected, -1, 0,numberOfScreens-1);
      break;
    case 4:
      if (elementPtr[screenSelected][elementSelected]->isSelectable())
      {
        locked = true;
      }
      break;
    }
  }
  else
  {
    /* Here if the selected Element is locked we execute it.
     The execute method responds with an integer, 
     -1 to unlock the element
     0 to do nothing and leave the element locked
     1 and above >> jump to [screen(x-1)] (where x is the returned integer)
     */
    int stateReturned = elementPtr[screenSelected][elementSelected]->execute(buttonPressed);
    if (stateReturned == -1)
    {
      locked = false;
    }
    if (stateReturned >0)
    {
       screenSelected = stateReturned-1; 
    }
  }

  if (buttonPressed != -1)
  {//-1 is the value for no button pushed. if it is not -1, then we must update since a button has been pushed

    update(); 
  }
  /*
  else{ //when the element is locked it will pass the button value to the element that was selected
    elementPtr[screenSelected][elementSelected]->execute(buttonPressed);
  }
  */
}
/*
  Prints to the LCD Screen the cursor which the User needs to see to select different Elements.
*/
void UIScreen:: printSelectionCursor()
{
  for (int i = 0; i < 4; i++)
  {
    inputPrinter(" ",0,i,false);
  }
  if (elementPtr[screenSelected][elementSelected]->isSelectable())
  {
    if (locked)
    {
     inputPrinter("*",0,elementSelected,false);

    }
    else
    {
     inputPrinter(">",0,elementSelected,false);
    }
  }
  else{
    inputPrinter("X",0,elementSelected,false);
  }
}

/*
  iterates through the elements and prints each of their values.
*/
void UIScreen :: update()
{
  for (int i=0; i <4; i++)
  {
    elementPtr[screenSelected][i]->updateLCD();
  } 
  printSelectionCursor();
}

/*
  Takes a target integer, adds adjustment to it (can be negative) and if the number
  goes under the lower bound will be set to the maximum. If it exceeds the upper bound
  it will be set to the minimum.
*/
void UIScreen :: circularIntAdjuster(int& target, int adjustment,int lower, int upper)
{
  target += adjustment;
  if (target < lower)
  {
    target = upper;
  }
  if (target > upper)
  {
    target = lower;
  }
}








