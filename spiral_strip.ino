#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// How many leds in each loop
//int loops[] = {19,30,46,58,76,71};
int loops[] = {12,18,30};

// delay between iterations
int delayval = 50; // delay in milisec

#define NUMOFCOLORS  3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int num_of_loops = sizeof(loops)/sizeof(int);
void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.    
}

void loop() {
//  first_loop_check();
  venta();

}

// --------------------
// branch_in_wind
// --------------------
void venta() {
  clearAll();  
  int color;  
  int current_led = 0;
  int move_color = 0;
  while (1) {
    current_led = 0;
    for (int c = 0 ; c < num_of_loops ; ++c) {
      for (int i = 0 ; i < loops[c] ; ++i) {      
        int new_i = (i+move_color)% loops[c]; // for move the color by 1 each iteration
        Serial.println(new_i);
        color = new_i*NUMOFCOLORS/loops[c];        
        pixels.setPixelColor(current_led,getColor(color,20));   
        current_led += 1;
      }               
    }
    pixels.show();   
    delay(delayval); 
    move_color++;
  }
  
  
  
}

// --------------------
// first_loop_check
// --------------------
void first_loop_check () {
  clearAll();
  int c;
  int l;
  int color;
  int current_led = 0;
  for (int c = 0 ; c < num_of_loops ; ++c) {
    color = (color +1)%NUMOFCOLORS;        
    for (l = 0 ; l < loops[c] ; ++l) {            
      pixels.setPixelColor(current_led,getColor(color,20));            
      current_led += 1;      
    }    
  }
  pixels.show();
  delay(delayval); 
}

//----------------------------
//  clearAll
//----------------------------
void clearAll() {
  for (int i = 0; i< NUMPIXELS; ++i) {
    pixels.setPixelColor(i,0);
  }

}
//----------------------------
//  getColor
//----------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// the level is how brigt will be tghe light (0 to 255).
uint32_t getColor(byte color, byte user_power) {
  if (user_power==0) return pixels.Color(0, 0, 0); 
  float power;
  float level;
 // Serial.println(color);
  level = (1.0*user_power)/100;
  if(color < NUMOFCOLORS/3) {
     power=1.0*color/(NUMOFCOLORS/3)*255;
     return pixels.Color(level*(255 - power), 0, level*power); 
  } else if(color < 2*NUMOFCOLORS/3) {
      color -= NUMOFCOLORS/3;
      power=1.0*color/(NUMOFCOLORS/3)*255;
  //    Serial.println(pixels.Color(0, level*power, level*(255 - power)));
      return pixels.Color(0, level*power, level*(255 - power));
  } else {
     color -= 2*NUMOFCOLORS/3;
     power=1.0*color/(NUMOFCOLORS/3)*255;
     return pixels.Color(level*power, level*(255 - power), 0);
  }
}

