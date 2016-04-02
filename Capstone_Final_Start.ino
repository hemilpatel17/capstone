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
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
//MP3 Player
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
#define DREQ            3      // VS1053 Data request, ideally an Interrupt pin

//Card CS
#define CARDCS          42     // Card chip select pin

//Program Specific defines
#define MESSAGE_SIZE    7     //TODO : maybe we dont need this, if we can dynamically get the message array length

#define SWITCH1         4 //blue   //if only need digtal then we have 4,5,6,7 and 2 available ,plenty of analog inputs available as well
#define SWITCH2         5 //red   //if need to be analog A0 and A1 should suffice

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
uint8_t counter = 3;//dueFlashStorage.read(1); //TODO : Use due flash storage to get this count (will need to initially write first)??
uint8_t volume = 20; //dueFlashStorage.read(2);/TODO : Use due flash storage to get this count  (will need to initially write first)??

//message holder array, if looping over works, I wont need have separate container
String messages [MESSAGE_SIZE] = {
  "message1",
  "message2",
  "message3",
  "message4",
  "message5",
  "message6",
  "message7"
  };

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
     //TODO : set this from the counter
     musicPlayer.setVolume(volume,volume);
     musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
    //Initialize LCD and UTFT
    Serial.println(F("Initializing LCD & UTFT")); 
    myGLCD.InitLCD();
    myGLCD.clrScr();
    myGLCD.setFont(SmallFont);
    myTouch.InitTouch();
    myTouch.setPrecision(PREC_EXTREME);
    myButtons.setTextFont(BigFont);

    pinMode(SWITCH1, INPUT_PULLUP);
    digitalWrite(SWITCH2, HIGH);
    //TODO : Add 'Hello I am pouchy come talk to me'
    //TODO : Add a picture of hello
    //TODO : Add a button that will say 'Settings'
    //TODO : if settting then go to admin menu else let it go to loop
    startUp();

}

void loop() {
    // put your main code here, to run repeatedly:
    //TODO : add the user program code here, no settings should be done here 
    
    userProgram();
    
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
    musicPlayer.playFullFile("MESSAGES/hello.mp3");
    
    //Checks if settings was pressed and if not go to loop(where main program happens) // will be done in setup 
    if(checkTouch(10,30,180,240)) {
        //TODO : go to settings page , if not, do not do anything and let the system go to loop
        settingsPage();
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
    //local variables
    char *tempImage, *tempAudio;
    String tempImageString, tempAudioString;
    uint8_t padVal;
    //do this for however many images there are in the sd card 
    //Display image
    //Read input from user
    //if there is an input from switch, audio out the message
    
    //for the last image, play random music till there is interrupt from user
    //lets hope that this works

    //TODO : YET TO TEST YET TO TEST YET TO TEST 
    for(int i = 0 ; i < MESSAGE_SIZE ; i++){
        tempImageString = "PICTURES/" + messages[i] + ".raw";
        tempImageString.toCharArray(tempImage, 50);
        Serial << "Temporary image name " << tempImage << "\n";
        tempAudioString = "MESSAGES/" + messages[i] + ".mp3";
        tempAudioString.toCharArray(tempAudio, 50);
        Serial << "Temporary audio file name " << tempAudio << "\n";
        myGLCD.clrScr();
        myFiles.load(0,0,800,480,tempImage,1,1);
        padVal = chkPads();
        if(padVal > 0) {      //if the value is zero there is no need to check for additional values
            if(padVal == 1){
                musicPlayer.playFullFile(tempAudio);
              }
            if(padVal == 2){
                subCategories(i); 
              }
            if(padVal == 3){
                //TODO : add extra functionalities, if subcategories are require and wait time increases by a lot
              }
          }
      }  
  }


void subCategories(uint8_t i) {
    //TODO input unsigned int to see what main category the program is in 
  }



//------------------Smaller modules------------------------//
//This module should do basic things like checkButtons, set counters etc.
//ALL PRE-LOWER/PRE-LIBRARY CODE GOES HERE , I am too lazy to create headers and other files or what not

//Check if there is a touch
boolean checkTouch(int x1, int x2, int y1, int y2){
    int x = 0;      //x and y are signed because value can be -1 when you read from the screen
    int y = 0;
    unsigned long startTime = millis();
    while((millis() - startTime) <= (counter * 1000)){  //counter * 1000 returns how many milliseconds to wait
      //TODO : maybe add if touch data available : if (touch.dataAvailable() == true)
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      if((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
        Serial << "Touch detected at co-ordinates x: " << x << " and y: " << y << "\n";
        return true;
        } 
    }
    return false;
  }

//check if there is touch at tactile pad
uint8_t chkPads() {
   uint8_t but1Pressed = 0;
   uint8_t but2Pressed = 0;
   unsigned long startTime = millis();
   while((millis() - startTime) <= (counter * 1000)) {    //keep checking the input for n seconds
      but1Pressed = digitalRead(SWITCH1);
      delay(100);   //maybe decrease the delay in future??, only after testing tho
      but2Pressed = digitalRead(SWITCH2);
      delay(100);
      if((but1Pressed == LOW) && (but2Pressed ==LOW)) {
          return 3;
        }
      if(but2Pressed ==LOW) {
          return 2;
        } 
      if(but1Pressed == LOW) {
          return 1;
        }
    }
    return 0;
  }

 
 
