/*
UISavePresetElement
 Allows the user to Save a preset from the User Menu
 Created by: Justin Read
 11/13/2014
 */
#include <Arduino.h>
#include <EEPROM.h>
#include "UISavePresetElement.h"
#include "UITextElement.h"
#define presetSize 44
#define numPads 6
UISavePresetElement :: UISavePresetElement(boolean isSelectable, 
int row, 
void (*inputPrinter)(String,int,int,boolean), 
String inputText,
int &inputSelected,
int &hiHatNoteValue,
DrumTrigger inPad[numPads]) : 
UITextElement(isSelectable, 
row, 
inputPrinter, 
inputText) //indicates that this class is derived from UIElement
{
  selectedPreset = &inputSelected;
  hiHatNote = &hiHatNoteValue;
  pad = inPad;
  displayedText = inputText;
  outputText = displayedText;
  state =0;
}

/*
  updateLCD >> This method takes prints the displayedText to the position(1,rowOccupied)
 It prints to 1 since 0 is reserved for the selection cursor. 
 True is sent because the line must be cleared prior to printing.
 */
void UISavePresetElement :: updateLCD()
{
  printToLCD(outputText, 1, rowOccupied, true);
}

/*
  Prompts the User if the slot is already full to confirm overwrite
 */
int UISavePresetElement :: execute(int buttonState)
{
  int address = *selectedPreset * presetSize;
  switch(state)
  {
  case 0://Start case, checks if the Selected Address is available
    if(EEPROM.read(address) == *selectedPreset)
    {
      state = 2; // bad state, data is already here, do we overwrite?
      outputText = "OverWrite?" ;
      updateLCD();
    }
    else
    {
      state = 1; // Good state, either data is corrupt or this slot is empty, write away! 
    }
    break;
  case 1: // Save state, go here to perform save Action
    savePreset(address);
    outputText = "Saved";
    updateLCD();
    state = 3; //We only execute once, so send exit command
    break;

  case 2: // Confirmation state, address already has data. 

    if (buttonState <4 && buttonState > -1) //if button up/down/left/right is pushed
    {
      outputText = "Cancelled";
      updateLCD();
      state = 3; //cancel
    }
    if (buttonState ==4)
    {
      state = 1; // go to save state
    }
    break;
  case 3: //Wait for putton press. This state just waits until we it OK to break out of the operation.
    if (buttonState == 4)
    {
      outputText= displayedText; //return text to original state.
      updateLCD();
      state = 0; //for next operation
      return -1; // if we hit OK here we just end 
    }
  }
  return 0;
}

/*
  savePreset()
 Performs the saving of the Preset
 */
void UISavePresetElement :: savePreset(int address)
{
  EEPROM.write(address++,*selectedPreset);//here we just write the preset number to the address as a verification that the slot is full.
  //EEPROM.write(address++,*midiChannel);//we then write the Midi Channel that the preset plays on. Almost always will be 10. but just to not lock people into something.
  EEPROM.write(address++,*hiHatNote);
  /*
   Now we loop a few times, the address int holds the location of the next write. it is post incremented each write.
   
   there are 7 writes per pad
   there are 6 pads
   42 pad writes,
   2 other writes
   Total size of a Save is 44 bytes.
   */

  for(int i =0; i <numPads; i++)
  {
    EEPROM.write(address++,pad[i].noteValue);
    //the thresholds are 10 bit numbers (0-1023), so they exceed a single byte. Here are operations to split them into 2 bytes.

    byte low = pad[i].thresholdLower; // gets the lower byte by truncating anything greater than 8 bits
    byte high = pad[i].thresholdLower >>8; // shifts the left byte over, then truncates again, preserving the high byte
    //then we write the lower threshold as 2 bytes
    EEPROM.write(address++,low);
    EEPROM.write(address++,high);

    //repeat this for the upper threshold
    low = pad[i].thresholdUpper; // gets the lower byte by truncating anything greater than 8 bits
    high = pad[i].thresholdUpper >>8; // shifts the left byte over, then truncates again, preserving the high byte
    EEPROM.write(address++,low);
    EEPROM.write(address++,high);
    EEPROM.write(address++,pad[i].minVelocity);
    EEPROM.write(address++,pad[i].maxVelocity);
  }
}

