//for Streaming output 
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

//Defines
//MP3 Player
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
#define DREQ            3      // VS1053 Data request, ideally an Interrupt pin
//Card CS
#define CARDCS          42     // Card chip select pin

//####Program Specific defines####//
//message size
#define MESSAGE_SIZE    7         //TODO : maybe we dont need this, if we can dynamically get the message array length

//switches
#define SWITCH1         4 //blue   //if only need digtal then we have 4,5,6,7 and 2 available ,plenty of analog inputs available as well
#define SWITCH2         5 //red   //if need to be analog A0 and A1 should suffice

//max track size
#define MAX_TRACK_SIZE  10


//Create an instance of UTFT + LCD screen
UTFT          myGLCD(EHOUSE70, 22,23,31,33);
UTouch        myTouch(25,26,27,29,30);
UTFT_SdRaw    myFiles(&myGLCD);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);

//Create an instance of Mp3 Library
Adafruit_VS1053_FilePlayer musicPlayer =
        Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
        
//Global variables
//uint8_t counter = 5;//dueFlashStorage.read(1); //TODO : Use due flash storage to get this count (will need to initially write first)??
//uint8_t volume = 20;//dueFlashStorage.read(2);//TODO : Use due flash storage to get this count  (will need to initially write first)??
//counter = constrain(counter,0,10);
//volume = constrain(volume,10,100);

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];


