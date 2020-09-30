#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define TIME_HEADER  "T"   
#define TIME_REQUEST  7  

void setup() {
  lcd.begin();
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  setSyncProvider(requestSync);
  Serial.println("Waiting for sync message");
  lcd.print("Loading...");
}

void loop() {
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW); 
  }
  delay(1000);
  lcd.setCursor(0, 0);
}

void digitalClockDisplay(){
  lcd.print("Time ");
  lcd.print(hour());
  printDigits(minute());
  printDigits(second());
  lcd.setCursor(0, 1);
  lcd.print("Date ");
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year()); 
   
  Serial.print(hour());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  lcd.print(":");
  if(digits < 10)
  lcd.print('0');
  lcd.print(digits);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600;

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) {
       setTime(pctime);
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; 
}
