/**
 * Lettura velocità encoder LPD3806
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// encoder
#define encoderPinA  2
#define encoderPinB  3

volatile long pos = 0;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  //display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text  
  display.cp437(true);         
  display.clearDisplay();  
}


unsigned long dt, t1;

void loop() {
  dt = millis() - t1;
  if (dt > 100) {
    //Serial.println(pos);
    display.clearDisplay();  
    display.setCursor(0, 0);
    display.print(pos);
    display.display();
    
    pos = 0;
    t1 = millis();
  }
}

void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoderPinA) == HIGH) {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoderPinB) == LOW) {
      pos = pos + 1;         // CW
    } else {
      pos = pos - 1;         // CCW
    }
  } else {// look for a high-to-low on channel A
    if (digitalRead(encoderPinB) == HIGH) {// check channel B to see which way encoder is turning
      pos = pos + 1;          // CW
    } else {
      pos = pos - 1;          // CCW
    }
  }  
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoderPinB) == HIGH) {
    // check channel A to see which way encoder is turning
    if (digitalRead(encoderPinA) == HIGH) {
      pos = pos + 1;         // CW
    } else {
      pos = pos - 1;         // CCW
    }
  } else { // Look for a high-to-low on channel B
    // check channel B to see which way encoder is turning
    if (digitalRead(encoderPinA) == LOW) {
      pos = pos + 1;          // CW
    } else {
      pos = pos - 1;          // CCW
    }
  }
}
