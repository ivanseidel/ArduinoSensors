#include "Ultrasonic.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 9, 4, 5, 6, 7);
Ultrasonic ultrasonic(12,13);

void setup() {
lcd.begin(16, 2);
lcd.print("testing...");
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ultrasonic.Ranging(CM));
  lcd.print("cm");
    
  delay(100);
}




