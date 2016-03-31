//Import all the needed libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <UTFT_SdRaw.h>
//#include <SD.h>
#include <SdFat.h>
SdFat sd;

//MP3 Player
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
#define DREQ            3      // VS1053 Data request, ideally an Interrupt pin

//Card CS
#define CARDCS 42     // Card chip select pin

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

//Create an instance of Mp3 Library
Adafruit_VS1053_FilePlayer musicPlayer =
        Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

//Create an instance of UTFT + LCD screen
UTFT          myGLCD(EHOUSE70, 22,23,31,33);
UTouch        myTouch(25,26,27,29,30);
UTFT_SdRaw    myFiles(&myGLCD);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);

//Global variables
uint8_t counter = 3; //TODO : Use due flash storage to get this count 


void setup() {
    Serial.begin(9600);
    while(!Serial) {
        ;//need for due
    }
    
    //Initialize the SD card
    while(!sd.begin(CARDCS,SPI_FULL_SPEED)) {
        Serial.println(F("Card failed to initialize, or not in the screen."));
    }
    Serial.println(F("Card Initialized successfully."));

    //Initialize Music Pllayer (VS1053b)
    while(!musicPlayer.begin()) {
      Serial.println(F("Music player failed to initialized, continuously trying to initialize it"));
      }
     
     musicPlayer.setVolume(20,20);
     musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
    //Initialize LCD and UTFT
    Serial.println(F("Initializing LCD & UTFT")); 
    myGLCD.InitLCD();
    myGLCD.clrScr();
    myGLCD.setFont(SmallFont);
    myTouch.InitTouch();
    myTouch.setPrecision(PREC_EXTREME);
    myButtons.setTextFont(BigFont);
 
    //TODO : Add 'Hello I am pouchy come talk to me'
    //TODO : Add a picture of hello
    //TODO : Add a button that will say 'Settings'
    //TODO : if settting then go to admin menu else let it go to loop
    

    startUp();

}

void loop() {
    // put your main code here, to run repeatedly:
    //TODO : add the main program code here, no settings should be done here 
    
    
}
//------------Startup module---------------// 
void startUp() {
    int adminPress = 0;
    int x = 0;
    int y = 0;
    myButtons.deleteAllButtons();
    myGLCD.clrScr();
    myGLCD.fillScr(255,255,255);
  
      //Start up screen and audio out for start up message
    myFiles.load(0,0,800,480,"PICTURES/hello.raw",1,1);
    int admin =  myButtons.addButton(599,0,200,50,"ADMIN");
    myButtons.drawButton(admin);
    
    Serial.println(F("Playing 'Hello Message.'"));
    musicPlayer.playFullFile("MESSAGES/track001.mp3");
    
    //Checks if settings was pressed and if not go to loop(where main program happens) // will be done in setup 
    if(checkTouch(10,30,180,240)) {
        //TODO : go to settings page , if not do not do anything and let the system go to loop
      }
}

//-----------------Bigger system modules----------------//
//This module is for settings page and user program only only
//NO LOWER LEVEL CODE HERE

//Settings Page
void settingsPage() {
  //TODO: add two buttons , one for volume and one for system counter, if volume is not needed then just do not add
  
  //Volume Button : Update volume counter

  //Delay : Add button, Update delay counter
  }
 
void userProgram()  {
  
    //do this for however many images there are in the sd card 
    //Display image
    //Read input from user
    //if there is an input from switch, audio out the message

    //for the last image, play random music till there is interrupt from user
    
}

//------------------Smaller modules------------------------//
//This module should do basic things like checkButtons, set counters etc.
//ALL PRE-LOWER/PRE-LIBRARY CODE GOES HERE , I am too lazy to create headers and other files or what not

//Check if there is a touch
boolean checkTouch(int x1, int x2, int y1, int y2){
    int x = 0;      //x and y are signed because value can be -1 when you read from the screen
    int y = 0;
    unsigned long startTime = millis();
    while((millis() - startTime) <= (counter * 1000)){  
      //TODO : maybe add if touch data available : if (touch.dataAvailable() == true)
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      if((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
        Serial.println(F("Touch detected"));
        return true;
        } 
    }
    return false;
}

 
 
