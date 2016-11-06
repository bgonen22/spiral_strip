#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// How many NeoPixels are in the first loop
#define NUMPIXELSFIRSTLOOP      15

// the diff btween two loops
#define DIFFBETWEENLOOPS      9

// delay between iterations
int delayval = 100; // delay in milisec

#define NUMOFCOLORS  3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  first_loop_check();
//  delay(delayval);
//  branch_in_wind();
//  delay(delayval);

}

// --------------------
// branch_in_wind
// --------------------
void branch_in_wind() {
  clearAll();
  int out = 0;
  int circle = 0;
  uint32_t color;
  int num_of_circles;
  int led;
  int base_led = 0;
  int step_num;
  while (1) {
    circle = 0;    
    out = 0;
    clearAll();
    while (!out) {
      color = pixels.Color(0,50,0);
      if (num_of_circles > 0 && (num_of_circles - circle < step_num)) {
        led = base_led + step_num+(num_of_circles - circle);
        Serial.println(step_num);
        Serial.println(num_of_circles);
        Serial.println(circle);
         Serial.println("----------");
        
      } else {
        led = base_led;      
      }
      //Serial.println(led);
      out = light_led_in_circle(led,circle,color);
      if (out) {
        num_of_circles = circle;
      }
      circle++;      
    }
    step_num++;
    if (num_of_circles > 0 ) {
      step_num = step_num  % num_of_circles;
    }    
    pixels.show();   
    delay(delayval); 
  }
}
// --------------------
// light_led_in_circle
// --------------------
int light_led_in_circle (int led, int circle, uint32_t color) {
  int index = circle * NUMPIXELSFIRSTLOOP + (DIFFBETWEENLOOPS*circle * (circle-1)/2) + led ;
  //Serial.println(index);
  if (index > NUMPIXELS) {
    return 1;
  }
  pixels.setPixelColor(index,color);
  return 0;
}
// --------------------
// first_loop_check
// --------------------
void first_loop_check () {
  clearAll();
  int start_loop = 1;
  int end_loop= NUMPIXELSFIRSTLOOP;
  int c = 0;
  int diff;
  uint32_t color;
  while (1) {
    color = getColor(c,50);
    for (int i = start_loop; i<= end_loop; ++i) {
      if (start_loop > NUMPIXELS ) {
        pixels.show();
        return;
      }
      pixels.setPixelColor(i,color);
    }
    //Serial.print("start_loop ");
//    Serial.print(start_loop);
    //Serial.print(" end_loop ");
    //Serial.println(end_loop);
    diff = end_loop-start_loop;
    start_loop = end_loop+1;
    end_loop = end_loop + diff+2;
    c= c+1%NUMOFCOLORS;    
  }
  
  
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

