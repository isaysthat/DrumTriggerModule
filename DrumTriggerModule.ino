
#include "DrumTrigger.h"     // Class for the Drum Trigger Objects
#include "Button.h"          // Class for the Button Objects
#include "UIScreen.h"        //Class for User Interface Menu
#include <LiquidCrystal.h>   //This library allows use of the LCD display
#include <EEPROM.h>          //this library allows for the use of EEPROM memory which presets will be saved in.

/* These two lines allow for altering variables within the Arduino's default code.
 Later in the program these functions will be used to change the rate at which the 
 Arduino can read analog pin values.
 */
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//Define makes use of actual code substitution. The text "nuTriggers" will be replaced with "6" when compiled.
//This is similar to declaring Constants but saves RAM.
#define numTriggers  6 // Six triggers for Six analog inputs.
#define numButtons  5 //the number of buttons on the physical platform. Up/Right/Down/Left/OK

//Data Fields
DrumTrigger pad[numTriggers] ; //the objects that represent the Physical Drum Triggers
Button button[numButtons]; // the objects that represent the Physical Buttons

int midiChannel = 10; //10 is the Default midi channel for Percussion instruments. 
int hiHatPad = 2; //This is the Trigger which corresponds to the HiHat. For my build it is Trigger 2.
int hiHatNoteValue = 42; // this is the default value for this value, which is loaded here instead of defaultTriggers()

int deBounceTime = 200; //number of milliseconds between button readings
int hysteresis = 100; //number of milliseconds for threshold of trigger to return to resting state
int hysteresisLock = 50; // number of milliseconds after a strike that the Triggers cannot produce more midi messages.
UIScreen LCD;  // This is the Object which represents the User Menu

LiquidCrystal lcd(2,3,4,5,6,7); //defines the LCD screen as residing on pins 2-7

/*
printRow()
 Takes a string, column, row, and boolean.
 If the boolean is true it will clear the row.
 It then prints the text at the Col,Row position.
 This method is declared before setup() because it is used in setup().
 */
void printRow(String text,int col, int row, boolean clearLine)
{
  if (clearLine)
  {
    lcdSetCursor(col,row);
    lcd.print("               ");
  }
  lcdSetCursor(col,row);
  lcd.print(text);
}

/*
lcdSetCursor()
 The 16X4 LCD module selected for this project is not completely compatible with
 the LiquidCrsytal Library used to control it. Since the library expects a 
 20X4 display the registers it addresses are off by 4 on the 3rd and 4th lines
 This function corrects for this.
 This method is declared before setup() because it is used in setup().
 */
void lcdSetCursor(int col, int row)
{
  if(row < 2)
  {
    lcd.setCursor(col,row);
  }
  else{
    lcd.setCursor(-4+col,row); 
  }
}

/*
setup()
 This is the initialization method that the Arduino IDE must have. Without a setup method the project will not compile.
 This initializes all of the data fields and objects used in the program.
 */
void setup()
{
  /*
  The following 3 lines of code are borrowed from microsmart.co.za/technical/2013/03/01/advanced-arduino-adc/
   They set the Analog to Digital Converter prescale clock to 16, meaning 16Mhz(clock speed of Arduino) / prescale(16) = 1Mhz
   The adc requires 25 cycles to resolve a value so 1Mhz/25 = approx 40,000 samples per second
   */
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;

  //This initializes the Serial pin of the board to communicate at 31250 baud. This is the speed defined by the General Midi Specification.
  Serial.begin(31250);

  defaultTriggers(); //sets the triggers to their default states.

  //initialize LCD screen
  lcd.begin(16, 4); //Tells the library it is 16 column by 4 row. 
  lcd.clear();      //Clears any data already in the LCD 
  LCD.init(printRow, pad,numTriggers, hiHatNoteValue); //sends data to the User Menu so that it can create pointers to the relevant data.
  LCD.update();  //Prints the User Interface Menu to the LCD.


  for (int i = 0; i <numButtons; i++) //this loop sets which pins the buttons are on.
  {
    button[i].pinNumber = 8+i; // 8 is the number of the pin of the first button.
    pinMode(button[i].pinNumber, INPUT); //sets button as input
    digitalWrite(button[i].pinNumber, HIGH);//turns on pull up resistor
  }
  //the hiHat pin must also get this treatment
  pinMode(13,INPUT);
  digitalWrite(13, HIGH);
}

/*
  loop()
 Similar to setup() this is a method which the Arduino IDE requires.
 This is similar to a java Main() method but also very different. 
 After setup() is called, this body of code is looped through forever
 */

void loop()
{

  triggerListener(); // listens to input and generates Midi Messages.
  LCD.buttonInput(buttonListener()); // sends the button reading to the LCD menu, menu handles when to update itself.
}


/*
  buttonListener()
 This method loops through the buttons and checks if they are pressed. It returns an integer.
 Depending on which button is pressed it will return 
 0 = up
 1 = right
 2 = down
 3 = left
 4 = OK
 */
int buttonListener()
{
  for(int i=0; i < numButtons; i++)
  {
    if (button[i].isPushed(deBounceTime))  //when button is pressed
    {
      return i;
    }
  }
  return -1; //indicates that no button was pressed
};

/*
  triggerListener()
 This is the heart of the Module. This method takes input from the Triggers and determines
 whether or not to produce a Midi Note.
 */
void triggerListener()
{

  for(int i =0; i < numTriggers ; i ++)
  {
    //two reads are performed since the Analog Inputs share a mutex. The first read locks the mutex, the next read gets the Value.
    pad[i].lastReading = analogRead(pad[i].pinNumber);//throw away
    pad[i].lastReading = analogRead(pad[i].pinNumber);//actual value

    double threshAdjustment;// represent the downward slope after a strike is made to deter false double taps.
    if (millis()  > pad[i].timeActivated + hysteresis)
    {//if the hysteresis time has passed there is no threshold adjustment
      threshAdjustment = 0;
    }
    else
    {//otherwise it is a downward slope starting at the upper threshold and moving back down to the lower.
      double range = (pad[i].thresholdUpper - pad[i].thresholdLower);
      int timePassed = (millis() - (pad[i].timeActivated+hysteresisLock));
      threshAdjustment = (range/hysteresis) * (hysteresis - timePassed) ;
      //threshAdjustment = map(millis(),pad[i].timeActivated+hysteresisLock,pad[i].timeActivated+hysteresisLock+hysteresis,pad[i].currentMax,pad[i].thresholdLower);
      //threshAdjustment = range / 2;
    }
    if ((pad[i].lastReading > (pad[i].thresholdLower+threshAdjustment)) && (!pad[i].activated) && (millis() > pad[i].timeActivated + hysteresisLock))
    {//if a pad is struck hard enough begin listening for its peak.
      pad[i].activated = true;//we are now in an active peak seeking state.
      pad[i].timeActivated = millis();//the time we entered this state.
      pad[i].currentMax = pad[i].lastReading; // the maximum reading found thus far
    }

    if (pad[i].activated) //search for peak
    {
      if (millis() > pad[i].timeActivated + 3) // if we have searched for long enough
      {
        if (pad[i].currentMax > pad[i].thresholdUpper)
        {
          pad[i].currentMax = pad[i].thresholdUpper;//protects input from overloading
        }
        //map function converts a number from input min/max to output min/max
        int vel = map(pad[i].currentMax,pad[i].thresholdLower,pad[i].thresholdUpper,pad[i].minVelocity,pad[i].maxVelocity);

        byte midiOn = 144 + midiChannel -1; //144 is note on. the minus 1 is because midi counts channel from 0-15 not 1-16

        if (i!= hiHatPad) //if the pad played is not the designated HiHat
        {
          midiPlay(midiOn,pad[i].noteValue,vel); //turn on the note of the pad
          midiPlay(midiOn,pad[i].noteValue,0x00); //immediately turn the note off
        }
        else
        {//if the pad played IS the hihat, 
          if (digitalRead(13) == HIGH) //if the pedal is not pressed play default note
          {
            midiPlay(midiOn,pad[i].noteValue,vel); 
            midiPlay(midiOn,pad[i].noteValue,0x00); 
          }
          else
          {//if the pedal is pressed, we play the other note
            midiPlay(midiOn,hiHatNoteValue,vel); 
            midiPlay(midiOn,hiHatNoteValue,0x00); 
          }
        }

        pad[i].activated = false;//once we make the midi message the pad is deactivated.
      }
      else
      {//if we have not exceeded the time for peak searching we try to find the maximum peak.
        if(pad[i].lastReading > pad[i].currentMax)
        {
          pad[i].currentMax = pad[i].lastReading;
        }
      }
    }
  }
};

/*
  midiPlay()
 takes in a few integers and converts them into Serial Messages.
 */
void midiPlay(int cmd, int note, int velocity)
{
  Serial.write(cmd);
  Serial.write(note);
  Serial.write(velocity); 
}

/*
Sets the default trigger states. 
 Every threhsold is 100-1020
 Every Velocity is 30-127
 note values are General Midi Kit 1
 */
void defaultTriggers()
{
  pad[0].pinNumber = A0;
  pad[0].thresholdLower = 100;
  pad[0].thresholdUpper = 1020;
  pad[0].noteValue = 35; //kick
  pad[0].minVelocity = 30;
  pad[0].maxVelocity = 127;

  pad[1].pinNumber = A1;
  pad[1].thresholdLower = 100;
  pad[1].thresholdUpper = 1020;
  pad[1].noteValue = 38; //snare
  pad[1].minVelocity = 30;
  pad[1].maxVelocity = 127;

  pad[2].pinNumber = A2;
  pad[2].thresholdLower = 100;
  pad[2].thresholdUpper = 1020;
  pad[2].noteValue = 46; //42 is closed hihat, 46 is open
  pad[2].minVelocity = 30;
  pad[2].maxVelocity = 127;

  pad[3].pinNumber = A3;
  pad[3].thresholdLower = 100;
  pad[3].thresholdUpper = 1020;
  pad[3].noteValue = 43; //low tom
  pad[3].minVelocity = 30;
  pad[3].maxVelocity = 127;

  pad[4].pinNumber = A4;
  pad[4].thresholdLower = 100;
  pad[4].thresholdUpper = 1020;
  pad[4].noteValue = 50; //hitom
  pad[4].minVelocity = 30;
  pad[4].maxVelocity = 127;

  pad[5].pinNumber = A5;
  pad[5].thresholdLower = 100;
  pad[5].thresholdUpper = 1020;
  pad[5].noteValue = 49; //crash
  pad[5].minVelocity = 30;
  pad[5].maxVelocity = 127;
}




