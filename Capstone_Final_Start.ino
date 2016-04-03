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
#include "capstoneConfig.h"

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

//Initial setup when the system boots up
void setup() {
    Serial.begin(9600);
    while(!Serial) {
        ;//need for due
    }
    
    //Initialize the SD card
    while(!sd.begin(CARDCS,SPI_HALF_SPEED)) {
        Serial.println(F("Card failed to initialize, or not in the screen."));
    }
    Serial.println(F("Card Initialized successfully."));

    //Initialize Music Pllayer (VS1053b)
    while(!musicPlayer.begin()) {
      Serial.println(F("Music player failed to initialized, continuously trying to initialize it"));
      }

     //set music volume
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
    digitalWrite(SWITCH1, HIGH);
     pinMode(SWITCH2, INPUT_PULLUP);
    digitalWrite(SWITCH2, HIGH);
    startUp();

}

void loop() {
    // put your main code here, to run repeatedly:
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
    //play initial hello message
    musicPlayer.startPlayingFile("MESSAGES/hello.mp3");
    delay(2000);
    //Checks if settings was pressed and if not go to loop(where main program happens) // will be done in setup 
    if(checkTouch(10,30,180,240)) {
        //go to settings page , if not, do not do anything and let the system go to loop
        Serial.println(F("Setttings page goes here"));
        //settingsPage();
      }
}

//-----------------Bigger system modules----------------//
//This module is for settings page and user program only only
//NO LOWER LEVEL API/CODE HERE

//Settings Page
void settingsPage() {
  initializeSettingsPage();
    int x1,x2,y1,y2;
  //Volume Button : Update volume counter
   while(1) {
     if(checkTouch(x1,x2,y1,y2)) {    //check plus sign
        updateVolume("+");
      }else if(checkTouch(x1,x2,y1,y2)) {
        updateVolume("-");
      }else if(checkTouch(x1,x2,y1,y2)) {
        updateDelay("+");
      }else if(checkTouch(x1,x2,y1,y2)) {
        updateDelay("-");
      }else if(checkTouch(x1,x2,y1,y2)) {     //save
        break;
      }
   }
  }
 
void userProgram()  {
    //local variables
    char tempImage[50], tempAudio[50];
    String tempImageString, tempAudioString;
    uint8_t padVal;
    //do this for however many images there are in the sd card 
    //Display image
    //Read input from user
    //if there is an input from switch, audio out the message
    
    //for the last image, play random music till there is interrupt from user
    //lets hope that this works
    uint8_t messagePlayed = 0;
    //TODO : YET TO TEST YET TO TEST YET TO TEST 
    for(int i = 0 ; i < MESSAGE_SIZE ; i++){
       // unsigned long startTime = millis();
        //while((millis() - startTime) <= (counter * 1000)) {    //keep checking the input for n seconds
          
          tempImageString = "PICTURES/" + messages[i] + ".raw";
          tempImageString.toCharArray(tempImage, 50);
          Serial << "Temporary image name " << tempImage << "\n";
          tempAudioString = "MESSAGES/" + messages[i] + ".mp3";
          tempAudioString.toCharArray(tempAudio, 50);
          Serial << "Temporary audio file name " << tempAudio << "\n";
          myGLCD.clrScr();
          myFiles.load(0,0,800,480,tempImage,16);
          padVal = chkPads();
          //if(padVal > 0) {      //if the value is zero there is no need to check for additional values
              if(padVal == 1){
                  Serial.println(F("Pal pad 1 input detected, playing music file."));
                  musicPlayer.startPlayingFile(tempAudio);
                  delay(2000);
                   messagePlayed = 1;
                  //break;
                  
                }
              if(padVal == 2){
                  Serial.println(F("Pal pad 2 input detected, diving into subcategories."));
                  subCategories(i); 
                }
              if(padVal == 3){
                  Serial.println(F("Input on both pads was detected, yet to add functionality."));
                  //TODO : add extra functionalities, if subcategories are require and wait time increases by a lot
                  //ideas : emergency menu? , yes/no prompt? , skip the menu right away to the music file??
                }
            //}
          //}
          padVal = 0;
        }
  
        padVal = 0; //reset the pad value to 0 if not already 0
        //if there is pal pad press on switch 1, upload a calming picture and play a random music
        padVal = chkPads();
        if(padVal > 0 ) {  
            if(padVal == 1) {
                playRandomTrack(MAX_TRACK_SIZE);
              }
            if(padVal = 2) {
                stopMusic();
              }
            if(padVal == 3) {
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
      delay(200);   //maybe decrease the delay in future??, only after testing tho
      but2Pressed = digitalRead(SWITCH2);
      delay(200);
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

void playRandomTrack(uint8_t maxTracks) {
    char *trackname;
    String trackToPlay = "MUSIC/track00" +  random(maxTracks);
    trackToPlay.toCharArray(trackname,50);
    musicPlayer.startPlayingFile(trackname); 
  }

void stopMusic() {
    musicPlayer.stopPlaying();
  }

void initializeSettingsPage(){
  //TODO: add two buttons , one for volume and one for system counter, if volume is not needed then just do not add
   int volume_button_plus =  myButtons.addButton(599,0,200,50,"+");    //TODO : chagne where the volume buttons gets displayed on the screen
   int volume_button_minus =  myButtons.addButton(599,0,200,50,"-");    //TODO : chagne where the volume buttons gets displayed on the screen
  
   int delay_button_plus = myButtons.addButton(599,0,200,50,"+");   //TODO : chagne where the volume buttons gets displayed on the screen
   int delay_button_minus = myButtons.addButton(599,0,200,50,"-");   //TODO : chagne where the volume buttons gets displayed on the screen

   int save = myButtons.addButton(599,0,200,50,"SAVE");             ////TODO : chagne where the volume buttons gets displayed on the screen
   int x1,y1,x2,y2;                                   //TODO :Change it accordingly
   
   myButtons.deleteAllButtons();
   
   myButtons.drawButton(volume_button_plus);
   myButtons.drawButton(volume_button_minus);
   myButtons.drawButton(delay_button_plus);
   myButtons.drawButton(delay_button_minus);
   
   myGLCD.print("Volume",x1,y1);
   myGLCD.print("Delay",x2,y2);
  
  }
void updateDelay(char *sign) {
  if(sign == "+") {
        dueFlashStorage.write(2,volume++);
      }else{
        dueFlashStorage.write(2,volume--);
      }
  }

   
void updateVolume(char *sign) {
    if(sign == "+") {
        dueFlashStorage.write(2,volume++);
      }else{
        dueFlashStorage.write(2,volume--);
      }
  }


