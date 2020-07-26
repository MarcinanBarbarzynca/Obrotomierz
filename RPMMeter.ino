// EnableInterrupt - Version: Latest
#include <EnableInterrupt.h> //Fajna biblioteka w której przerwania dla odmiany działają
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ARDUINOPIN 2
volatile uint16_t licznik = 0;
volatile float HZ = 0;
const int analogInPin = A3; //Ten pin jest czasem używany do debugowania
int input = 2;
volatile unsigned long czas = 0;
volatile unsigned long T = 0;

void funkcja_przerwania() {
  licznik++;
  HZ=1/(float(millis()-T)*1000);
  T = millis();
}
void wypisz_HZ() {
    //Serial.println(wartosc); // 
   // char buff[10]; //Użyj tych wartości jeżeli ten lcd nie będzie chciał wyświetlać typu float
   // char buff_sixty[10];
   // dtostrf(HZ, 1, 3, buff);  //Przekonwertowuje float do stringa
   // dtostrf(HZ*60, 1, 3, buff_sixty);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(HZ,10);
  //lcd.print(buff);
  lcd.print(" HZ");
  lcd.setCursor(0, 1);
  lcd.print(HZ*60,10);
  lcd.print(" RPM");
  HZ = 0;
}
void setup() {
  pinMode(input, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Obrotosciomierz");
  lcd.setCursor(0, 1);
  lcd.print("Juz dzialam");
  delay(2000);
  enableInterrupt(ARDUINOPIN, funkcja_przerwania, FALLING);
}
void loop() {
  state = digitalRead(input);
  if (state == 1) {
    lcd.setCursor(11, 0);
    lcd.print("DREWN");
  } else {
    lcd.setCursor(11, 0);
    lcd.print("METAL");
  }
  lcd.setCursor(11, 1);
  lcd.print(licznik);
  if (millis() - czas > 1000) {
    czas = millis();
    wypisz_HZ();
  }
}
