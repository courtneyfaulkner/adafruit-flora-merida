#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
//#include <CapPin.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, PIN, NEO_GRB + NEO_KHZ800);
CapacitiveSensor   cs_9_10 = CapacitiveSensor(9,10);
//CapPin cPin_10 = CapPin(10);
uint16_t state;
long previousMillis = 0;

//uint8_t myFavoriteColors[][3] = {{200,   0, 200},   // purple
//                                 {200,   0,   0},   // red 
//                                 {200, 200, 200},   // white
//                               };
uint8_t myFavoriteColors[][3] = {{63, 63, 63}   // white
                               };
// don't edit the line below
#define FAVCOLORS sizeof(myFavoriteColors) / 3
 
// mess with this number to adjust TWINklitude :)
// lower number = more sensitive
#define MOVE_THRESHOLD 45


void setup() {
  cs_9_10.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
//  Serial.println(cs_9_10.capacitiveSensor(30));
  
//    long start = millis();
//    long total1 =  cs_9_10.capacitiveSensor(30);
 
//    if (total1 > 2000){
//      digitalWrite(7, HIGH);
//    } else {
//      digitalWrite(7, LOW);
//    }      
  
//    Serial.print(millis() - start);        // check on performance in milliseconds
//    Serial.print("\t");                    // tab character for debug windown spacing
// 
//    Serial.println(total1);                  // print sensor output 1
  
//  strip.setPixelColor(0, strip.Color(255, 255, 255));

//  state = checkForTouch(cs_9_10.capacitiveSensor(30), state);
  state = checkForTouch();

  if (state == 0) {
    for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
    strip.show();
  } else if (state == 1) {
    for(uint16_t i=0; i<63; i++) {
        strip.setPixelColor(0, strip.Color(0, 0, i));
        strip.show();
        delayWhileChecking(20);
        if (state != 1) return;
    }
    
    delayWhileChecking(500);
    if (state != 1) return;
  
    for(uint16_t i=63; i<255; i++) {
        strip.setPixelColor(0, strip.Color(0, 0, i));
        strip.show();
        delay(1);
    }
    
    for(uint16_t i = 255; i > 0; i--) {
        strip.setPixelColor(0, strip.Color(0, 0, i));
        strip.show();
        delay(1);
    }
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
  } else if (state == 2) {
    flashRandom(5, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
    flashRandom(5, 3);  // second number is how many neopixels to simultaneously light up
    flashRandom(5, 2);
  }
}

void delayWhileChecking(unsigned int interval) {
  do {
    unsigned long currentMillis = millis();
    state = checkForTouch();
    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      return;
    }
  } while(true);
  
}

uint16_t checkForTouch() {
//  long sense = cPin_10.readPin(2000);
  long sense = cs_9_10.capacitiveSensor(30);
//  Serial.print(state);
//  Serial.print("\t");
//  Serial.println(sense);
  if (sense > 2500) {
    strip.setPixelColor(0, strip.Color(24, 24, 24));
    strip.show();
    delay(15);
    strip.setPixelColor(0, 0);
    strip.show();
    delay(500);
    if (state == 0) {
      return 1;
    } else if (state == 1) {
      return 2;
    } else {
      return 0;
    }
  }
  return state;
}

void flashRandom(int wait, uint8_t howmany) {
 
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random favorite color!
    int c = random(FAVCOLORS);
    int red = myFavoriteColors[c][0];
    int green = myFavoriteColors[c][1];
    int blue = myFavoriteColors[c][2]; 
 
    // get a random pixel from the list
    int j = random(strip.numPixels());
    Serial.print("Lighting up "); Serial.println(j); 
    
    // now we will 'fade' it in 5 steps
    for (int x=0; x < 5; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }
    // & fade out in 5 steps
    for (int x=5; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }
  }
  // LEDs will be off when done (they are faded to 0)
}

