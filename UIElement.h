/*
UIElement
  This library is the Base class of the User Interface Element Class Hierarchy.
  This holds the default and virtual methods that each UI row would contain. 
  The methods here are abstract and overridden by other subclasses except
  for isSelectable which returns the value of selectable.
Created by: Justin Read
10/20/2014
*/
#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Arduino.h>
class UIElement
{
public://allows any class in the project to manipulate it
  UIElement();
  int rowOccupied;
  boolean selectable;
  //printToLCD is a method pointer. allows any UIElement to print using this method.
  void (*printToLCD)(String,int,int,boolean);
  //Virtual Functions are overridden by derived classes.
  virtual int execute(int buttonPressed); 
  virtual void updateLCD(); 
  virtual boolean isSelectable();
};

#endif
