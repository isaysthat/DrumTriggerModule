
/*
UILODADresetElement
 Created by: Justin Read
 11/13/2014
 */

#include <Arduino.h>
#include <EEPROM.h>
#include "UILoadPresetElement.h"
#include "UITextElement.h"
#define presetSize 44
#define numPads 6

UILoadPresetElement :: UILoadPresetElement(boolean isSelectable, 
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
  outputText = inputText;
  displayedText = inputText;
  state =0;
}

/*
  updateLCD >> This method takes prints the displayedText to the position(1,rowOccupied)
 It prints to 1 since 0 is reserved for the selection cursor. 
 True is sent because the line must be cleared prior to printing.
 */
void UILoadPresetElement :: updateLCD()
{
  printToLCD(outputText, 1, rowOccupied, true);
}

/*
  Prompts the user with confirmation if the slot selected is empty or if loading was successful.
 */
int UILoadPresetElement :: execute(int buttonState)
{
  int address = *selectedPreset * presetSize;
  switch(state)
  {
  case 0://Start case, checks if the Selected Address is available
    if(EEPROM.read(address) == *selectedPreset) //255 is the default state, its empty
    {
      state = 1; // good state, data is already here, load it
      loadPreset(address);
      outputText = "Loaded" ;
      updateLCD();
    }
    else
    {
      state = 1; // bad state, either data is corrupt or this slot is empty.
      outputText = "Empty/Corrupt" ;
      updateLCD();
    }
    break;
  case 1: //Wait for putton press. This state just waits until we it OK to break out of the operation.
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
  loadPreset
 This function performs the load by accessing the EEPROM memory.
 */
void UILoadPresetElement :: loadPreset(int address)
{
  address++;//throw away the first address since it only holds the preset number as an indication that it is written to.
  *hiHatNote = EEPROM.read(address++);
  //*midiChannel = EEPROM.read(address++);//set the midichannel pointer to the value read.

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
    pad[i].noteValue = EEPROM.read(address++);
    //the thresholds are 10 bit numbers (0-1023), so they exceed a single byte. Here are operations to split them into 2 bytes.

    byte low ; // gets the lower byte by truncating anything greater than 8 bits
    byte high; // shifts the left byte over, then truncates again, preserving the high byte
    low = EEPROM.read(address++);
    high = EEPROM.read(address++);
    pad[i].thresholdLower =( high << 8) + low;

    //then we write the lower threshold as 2 bytes
    low = EEPROM.read(address++);
    high = EEPROM.read(address++);
    pad[i].thresholdUpper =( high << 8) + low;

    pad[i].minVelocity = EEPROM.read(address++);
    pad[i].maxVelocity = EEPROM.read(address++);
  }
}

