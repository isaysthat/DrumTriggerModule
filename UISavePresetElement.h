/*
UISavePresetElement
 This class is derived from UITextElement. 
 Created by: Justin Read
 11/24/2014
 */

#ifndef UISAVEPRESETELEMENT_H
#define UISAVEPRESETELEMENT_H
#include <Arduino.h>
#include "UITextElement.h"
#include "DrumTrigger.h"
#define numPads 6
class UISavePresetElement : public UITextElement //indicates that this class is derived from UIElement
{
public://allows any class in the project to manipulate it
  UISavePresetElement(boolean isSelectable, int row, void (*inputPrinter)(String,int,int,boolean), String inputText,int &inputSelected, int &hiHatNoteValue, DrumTrigger inpad[numPads]);
  int execute(int buttonState);
  void updateLCD();
  String displayedText;
  String outputText;
  int *selectedPreset;
  int *hiHatNote;
  int state;
  DrumTrigger *pad;
  void savePreset(int address);
  //boolean isSelectable(); 
  //boolean selectable;
};

#endif

