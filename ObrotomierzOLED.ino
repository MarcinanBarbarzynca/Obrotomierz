#include "lcdgfx.h"
#include <EnableInterrupt.h> //Fajna biblioteka w której przerwania dla odmiany działają
#include <avr/wdt.h>

DisplaySSD1306_128x64_I2C display(-1);

#define ARDUINOPIN 2

volatile uint16_t licznik = 0;
volatile float HZ = 123.456;
const int analogInPin = A3; //Ten pin jest czasem używany do debugowania
const int input = 2;
volatile unsigned long czas = 0;
volatile unsigned long T = 0;
int state = 0;//Zmienna potrzebna do loopa
int do_resetu = 0;

void funkcja_przerwania() {
  licznik++;
  //HZ=133.123;
  HZ = 1000 / (float(millis() - T));
  T = millis();
}

void(* resetFunc) (void) = 0; //Funkcja automatycznego resetu

void wypisz_HZ() {
  disableInterrupt(ARDUINOPIN);
  char buff[10]; //Użyj tych wartości jeżeli ten lcd nie będzie chciał wyświetlać typu float
  char buff_sixty[10];
  dtostrf(HZ, 1, 3, buff);  //Przekonwertowuje float do stringa
  dtostrf(HZ * 60, 1, 3, buff_sixty);
  display.fill(0x00);

  display.printFixedN(0,  0, buff, STYLE_BOLD, FONT_SIZE_2X);
  display.printFixedN(0,  32, buff_sixty, STYLE_BOLD, FONT_SIZE_2X);
  //HZ = 0;
  enableInterrupt(ARDUINOPIN, funkcja_przerwania, FALLING);
}


void setup() {
  pinMode(input, INPUT);
  wdt_enable(WDTO_1S); //aktywacja waczdoga dla funkcji init ktora sie wiesza i robi kwadraty
    display.begin();
    display.fill(0x00);
    display.setFixedFont(ssd1306xled_font6x8);
  display.printFixedN (0, 0, "Obrotomiez", STYLE_BOLD, FONT_SIZE_2X);
  display.printFixedN (0, 32, "Juz dziala", STYLE_BOLD, FONT_SIZE_2X);
  wdt_reset();
  wdt_disable();
  delay(1000);
  enableInterrupt(ARDUINOPIN, funkcja_przerwania, FALLING);
}
void loop() {
   state = digitalRead(input);
  if (state == 1) {
    display.printFixed (0,  16, "METAL", STYLE_NORMAL);
    
  } else {
    display.printFixed (0,  16, "DREWN", STYLE_NORMAL);
  }
    //display.printFixed (0,  24, licznik, STYLE_NORMAL);
  if (millis() - czas > 1000) {
    czas = millis();
    wypisz_HZ();
    do_resetu++;
    /*if (do_resetu == 60) {
      do_resetu = 0;
      resetFunc();
    }*/

  }
}
