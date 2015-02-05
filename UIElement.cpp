/*
This is the base class for all of the UIElements. 
*/

#include "UIElement.h"
#include <LiquidCrystal.h>
UIElement::UIElement() 
{
}
int UIElement:: execute(int buttonPressed)
{
}
void UIElement:: updateLCD()
{
}
boolean UIElement:: isSelectable()
{
  return selectable;
}

