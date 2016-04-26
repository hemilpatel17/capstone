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
    "change",
    "talk",
    "yes",
    "no",
    "stretch",
    "walk"
};

String subCat[MESSAGE_SIZE] [SUB_MAX_SIZE] = {
    "pain", "sick", "",
    "knock", "", "",
    "", "", "",
    "", "", "",
    "music", "pillow", "side",
    "nurse", "", ""
};
uint8_t counter = dueFlashStorage.read(1);
uint8_t volume = dueFlashStorage.read(2);
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
    
    counter = constrain(counter,3,10);
    volume = constrain(volume,20,60);
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
    myFiles.load(0,0,800,480,"PICTURES/hello.raw",16);
    int admin =  myButtons.addButton(599,0,200,50,"ADMIN");
    myButtons.drawButton(admin);
    
    Serial.println(F("Playing 'Hello Message.'"));
    //play initial hello message
    delay(100); 
    musicPlayer.startPlayingFile("MESSAGES/hello.mp3");
    delay(3000);
    //Checks if settings was pressed and if not go to loop(where main program happens) // will be done in setup
    if(checkTouch(10,30,180,240,counter*100)) {
        //go to settings page , if not, do not do anything and let the system go to loop
        Serial.println(F("Setttings page goes here"));
        settingsPage();
    }
}

//-----------------Bigger system modules----------------//
//This module is for settings page and user program only only
//NO LOWER LEVEL API/CODE HERE

//Settings Page
void settingsPage() {
    initializeSettingsPage();
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
            delay(2000);                  //TODO : if audio files are bigger than 2 seconds increase this delay

            while(chkPadsForDelay() != 0){
              delay(10);
            }
           
            
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
    myGLCD.clrScr();
    myFiles.load(0,0,800,480,"PICTURES/playmusic.raw",16);
   padVal = chkPads();
   Serial << "pad value for music is" << padVal << "\n";
   if(padVal == 1) {
    playRandomTrack();
    while(1){ 
        padVal = chkPads();
        if(padVal == 2) {
            stopMusic();
            break;
        }
      }
    }
   
}


void subCategories(uint8_t i) {
    //TODO input unsigned int to see what main category the program is in
    //swtich case??
    //change for git???
    char tempImage[50], tempAudio[50];
    String tempImageString, tempAudioString;
    uint8_t padVal;
    for (int j = 0 ; j < SUB_MAX_SIZE ; j++){
      Serial << "I get here\n" ;
      if(subCat[i][j].length() != 0){
        Serial << "Subcategory is " << subCat[i][j] << "\n";
        tempImageString = "PICTURES/" + subCat[i][j] + ".raw";
        tempImageString.toCharArray(tempImage, 50);
        Serial << "Temporary image name " << tempImage << "\n";
        tempAudioString = "MESSAGES/" + subCat[i][j] + ".mp3";
        tempAudioString.toCharArray(tempAudio, 50);
        Serial << "Temporary audio file name " << tempAudio << "\n";
        myGLCD.clrScr();
        myFiles.load(0,0,800,480,tempImage,16);
        
        padVal = chkPads();
        if(padVal == 1){
            Serial.println(F("Pal pad 1 input detected, playing music file."));
            musicPlayer.startPlayingFile(tempAudio);
            delay(2000);                  //TODO : if audio files are bigger than 2 seconds increase this delay

            while(chkPadsForDelay() != 0){
              delay(10);
            } 
        }
        if(padVal == 2){
            Serial.println(F("Pal pad 2 input detected, diving into subcategories."));
            return;
        }
       }
      }
}



//------------------Smaller modules------------------------//
//This module should do basic things like checkButtons, set counters etc.
//ALL PRE-LOWER/PRE-LIBRARY CODE GOES HERE , I am too lazy to create headers and other files or what not

//Check if there is a touch
boolean checkTouch(int x1, int x2, int y1, int y2,uint8_t timeToWaitInMillis){
    int x = 0;      //x and y are signed because value can be -1 when you read from the screen
    int y = 0;
    unsigned long startTime = millis();
    while((millis() - startTime) <= timeToWaitInMillis){  //counter * 1000 returns how many milliseconds to wait
        if(myTouch.dataAvailable()){
            myTouch.read();
            x=myTouch.getX();
            y=myTouch.getY();
            if((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
                Serial << "Touch detected at co-ordinates x: " << x << " and y: " << y << "\n";
                return true;
            }
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

uint8_t chkPadsForDelay() {
    uint8_t but1Pressed = 0;
    uint8_t but2Pressed = 0;
    unsigned long startTime = millis();
    while((millis() - startTime) <= (500)) {    //keep checking the input for n seconds
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
void playRandomTrack() {
    musicPlayer.stopPlaying();
    char trackname[50];
    String trackNum = String(random(MAX_TRACK_SIZE));
    Serial << "File num to play " << trackNum;
    String trackToPlay = "MUSIC/track00" +  trackNum + ".mp3";
    Serial << "track to play " << trackToPlay << "\n";
    trackToPlay.toCharArray(trackname,50);
    Serial << "trackname " << trackname << "\n";
    musicPlayer.startPlayingFile(trackname);
}

void stopMusic() {
    musicPlayer.stopPlaying();
}

void initializeSettingsPage(){
    //TODO: add two buttons , one for volume and one for system counter, if volume is not needed then just do not add
    myGLCD.clrScr();
    myGLCD.setFont(BigFont);
    myButtons.deleteAllButtons();
    myGLCD.print("VOLUME",200,75);
    myGLCD.print("DELAY",575,75);
    // myGLCD.setFont(SmallFont);
    int volume_button_plus =  myButtons.addButton(150,99,200,50,"+");    //TODO : chagne where the volume buttons gets displayed on the screen
    int volume_button_minus =  myButtons.addButton(150,200,200,50,"-");    //TODO : chagne where the volume buttons gets displayed on the screen
    
    int delay_button_plus = myButtons.addButton(500,99,200,50,"+");   //TODO : chagne where the volume buttons gets displayed on the screen
    int delay_button_minus = myButtons.addButton(500,200,200,50,"-");   //TODO : chagne where the volume buttons gets displayed on the screen
    
    int save = myButtons.addButton(325,300,200,50,"SAVE");             ////TODO : chagne where the volume buttons gets displayed on the screen
    int but;                                  //TODO :Change it accordingly
    int x, y;
    myButtons.drawButton(volume_button_plus);
    myButtons.drawButton(volume_button_minus);
    myButtons.drawButton(delay_button_plus);
    myButtons.drawButton(delay_button_minus);
    myButtons.drawButton(save);
    
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.printNumI(60 - volume, 215, 150, 2,'0');
    myGLCD.printNumI(counter, 570, 150, 2,'0');
    
    while(1) {
        //save
        //TODO : if touch available ,kills the execution time
        if(checkTouch(210,260,90,160,10)){
            dueFlashStorage.write(2,volume);
            dueFlashStorage.write(1,counter);
            break;
            //Vol+
        }else if(checkTouch(50,100,40,100,50)){
            updateVolume("+");
            myGLCD.printNumI(60 - volume, 215, 150, 2,'0');
            //Vol-
        }else if(checkTouch(130,180,40,100,50)){
            updateVolume("-");
            myGLCD.printNumI(60 - volume, 215, 150, 2, '0');
            //D+
        }else if(checkTouch(60,100,150,220,50)){
            updateDelay("+");
            myGLCD.printNumI(counter, 570, 150, 2,'0');
            //D-
        }else if(checkTouch(130,180,150,220,50)){
            updateDelay("-");
            myGLCD.printNumI(counter, 570, 150, 2,'0');
        }
    }
    Serial.println("Menu saved");
    Serial << "delay : " << counter << " volume " << volume << "\n";
    musicPlayer.setVolume(volume,volume);
}

void updateDelay(char *sign) {
    if(sign == "+") {
        counter = counter + 1;
        counter = constrain(counter, 3, 10);
    }else{
        counter = counter - 1;
        counter = constrain(counter, 3, 10);
    }
}


void updateVolume(char *sign) {
    if(sign == "+") {
        volume = (volume - 5);
       
        volume = constrain(volume,20,60);
    }else{
        volume = (volume + 5);
        volume = constrain(volume,20,60);
    }
    Serial.println(volume);
}


