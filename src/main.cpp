#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//Constants declared
#define ConstLedPin 1
#define ConstLedCount 16
#define ConstBrightness 75

//Variables declared
Adafruit_NeoPixel ring(ConstLedCount, ConstLedPin, NEO_GRBW + NEO_KHZ800);
char serialInput;
int serialInputAvailable;
uint32_t color;
uint8_t waitTimeMS;

void setup() {
  // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
 // END of Trinket-specific code.

  Serial.begin(9600);
  ring.begin();
  ring.show();
  ring.setBrightness(ConstBrightness);
}

void pulseBlue(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    ring.fill(ring.Color(0, 0, j, ring.gamma8(j)));
    ring.show();
    delay(wait);
  }
    for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    ring.fill(ring.Color(0, 0, j, ring.gamma8(j)));
    ring.show();
    delay(wait);
  }
}

char readInput() {
  serialInputAvailable = Serial.available();
   if (serialInputAvailable > 0) {
    serialInput = Serial.read();
    // Serial.println(serialInput);
  } else if (serialInputAvailable == 0) {
    pulseBlue(5);
  } else {
    Serial.println("No Serial");
  }

  return serialInput;
}

// void displayInput(char serialInput) {
//   if (newInputRecieved == true) {
//     Serial.println("Input: " + serialInput);
//   }
// }

void chaseColor(uint32_t color, uint8_t waitTimeMS) {
    for(int i = 0; i < ring.numPixels(); i++) { // For each pixel in strip
    ring.setPixelColor(i, color); //  Set pixel's color
    ring.show(); //  Update strip to match
    delay(waitTimeMS); //  Pause for a moment
  }
}

void processInput(char serialInput) {
  switch(serialInput) {
    case 'A':
      chaseColor(ring.Color(255, 0, 0), 100);
      Serial.println("A pressed");
    break;

    case 'S':
      chaseColor(ring.Color(0, 255, 0), 50);
      Serial.println("S pressed");
    break;

    case 'D':
      chaseColor(ring.Color(0, 0, 255), 25);
      Serial.println("D pressed");
    break;

    case 'F':
      chaseColor(ring.Color(125, 125, 10), 13);
      Serial.println("F pressed");
    break;

    default: 
      pulseBlue(5);

  }
}

void loop() {
  readInput();
  processInput(serialInput);
  pulseBlue(5);
}