
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
    Serial.begin(9600);
    while(!Serial) {
        ;//need for due
    }
    pinMode(4, INPUT_PULLUP);
    digitalWrite(4, HIGH);
     pinMode(5, INPUT_PULLUP);
    digitalWrite(5, HIGH);
 uint8_t but1Pressed = 0;
   uint8_t but2Pressed = 0;
   unsigned long startTime = millis();
   while((millis() - startTime) <= (3 * 1000)) {    //keep checking the input for n seconds
      but1Pressed = digitalRead(4);
      delay(100);   //maybe decrease the delay in future??, only after testing tho
      but2Pressed = digitalRead(5);
      delay(100);
      if((but1Pressed == LOW) && (but2Pressed ==LOW)) {
         Serial.println(F("Button 1 and 2 presssed"));
        }
      if(but2Pressed ==LOW) {
          Serial.println(F("Button 2 pressed"));
        } 
      if(but1Pressed == LOW) {
         Serial.println(F("Button 1 pressed"));
        }
    }
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
