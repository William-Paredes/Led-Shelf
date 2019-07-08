#include "FastLED.h"
#define LED_NUM  117
#define DATA_PIN 6 
#define LED_TYPE WS2812B
#define BRIGHTNESS 255
#define COLOR_ORDER GRB
const int button = 4; 
int LED_MODE = 0; 
int buttonState = 0; 
CRGB leds[LED_NUM];
// Timing var
unsigned long previous_millis = 0; 
// constants won't change:
const long interval = 4;
const long color_interval = 50;
unsigned long color_millis = 0; 

boolean button_pressed = false; 
boolean toggle = false; 
int debounce = 0; 
static uint8_t startIndex = 0;
static uint8_t color_cycle = 0;
// First Row has 27
// Second Row has 30 LED
// Third Row has 30 LED 
// Fourth Row has 30 LED
int rgb_sequence[LED_NUM] = {     56,   57,   116, 
                             0,   55,   58,   115, 
                             1,   54,   59,   114,
                             2,   53,   60,   113,
                                  52,   61,   112, 
                             3,   51,   62,   111,
                             4,   50,   63,   110,
                             5,   49,   64,   109, 
                             6,   48,   65,   108,
                             7,   47,   66,   107,
                             8,   46,   67,   106,
                             9,   45,   68,   105,
                             10,  44,   69,   104,
                             11,  43,   70,   103,
                             12,  42,   71,   102,
                             13,  41,   72,   101,
                             14,  40,   73,   100,
                             15,  39,   74,   99,
                             16,  38,   75,   98,
                             17,  37,   76,   97,
                             18,  36,   77,   96,
                             19,  35,   78,   95,
                             20,  34,   79,   94,
                             21,  33,   80,   93,
                             22,  32,   81,   92,
                                  31,   82,   91,
                             23,  30,   83,   90,
                             24,  29,   84,   89,
                             25,  28,   85,   88,
                             26,  27,   86,   87 
                            }; 
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


void setup() {
  // put your setup code here, to run once:
  delay(3000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds,LED_NUM).setCorrection( TypicalLEDStrip ); 
  FastLED.setBrightness(BRIGHTNESS); // global brightness 
  pinMode(button, INPUT); 
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  Serial.begin(9600);
  unsigned long time_color = millis(); 
  
  if(digitalRead(button) == HIGH)
 {
  debounce++; 
  Serial.print("DEBOUNCE: ");
  Serial.println(debounce);
 }
 else {
  debounce = 0; 
  button_pressed = false; 
  toggle = false; 
 }

 if(debounce >= 1){
  button_pressed = true; 
 }

 if(button_pressed == true && toggle == false)
 {
  Serial.println("Button: PRESSED");
  toggle = true; 
  LED_MODE++; 
 }
    switch(LED_MODE)
    {
      case 0: {
                do_warm(); 
                break;
              }  
      case 1: {
                do_cool(); 
                break;
              }
      case 2: { 
              
               do_rgb_stream(rgb_sequence[startIndex],color_cycle ); 
              break; 
              }
    }

    if(LED_MODE > 2) LED_MODE = 0; 
    FastLED.show();
    
    if (time_color - color_millis >= color_interval){
    color_millis = time_color; 
    startIndex++;
    color_cycle += 3;
    } 
    
    if(startIndex >= LED_NUM){
      startIndex = 0;
    }
}

void do_warm(){
    for(int i = 0; i < LED_NUM; i++) {
    leds[i].red = 255; 
    leds[i].green = 147;
    leds[i].blue = 41; 
  }
}

void do_cool(){
  for(int i = 0; i < LED_NUM; i++) {
    leds[i].red = 201; 
    leds[i].green = 226;
    leds[i].blue = 255; 
  }
}


void do_rgb_stream(uint8_t colorIndex, uint8_t color_value){
    unsigned long currentMillis = millis();
    
    if (currentMillis - previous_millis >= interval) {
    // save the last time you blinked the LED
    previous_millis = currentMillis;
    leds[colorIndex] = ColorFromPalette( currentPalette, color_value, BRIGHTNESS, currentBlending);
    }
  
}
