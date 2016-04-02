#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    while(!Serial) {
        ;//need for due
    }
    
    
  uint8_t count1 = dueFlashStorage.read(1);
  uint8_t count2 = dueFlashStorage.read(2);
  Serial << "counn1 " << count1 << "\n";
  Serial << "counn2 " << count2 << "\n";
  //dueFlashStorage.write(1, 4);
  //dueFlashStorage.write(2, 10);
  //count1 = dueFlashStorage.read(1);
  count2 = dueFlashStorage.read(2);
  Serial << "counn1 " << count1 << "\n";
  Serial << "counn2 " << count2 << "\n";
}

void loop() {
  // put your main code here, to run repeatedly:

}
