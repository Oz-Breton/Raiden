/*
  Current Working Version
*/
//reset by connecting reset to ground
#include <Adafruit_TiCoServo.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    3

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 7

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN);

bool condition = true;
bool con = true;
unsigned long timer = 0;
int pos_1 = 180;
int pos_2 = 0;
int servo_pin1 = 9;
int servo_pin2 = 10;
int inPin = 6;         // the number of the input pin
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
int flashOn = 500;
int flashOff = 1000;


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;
Adafruit_TiCoServo myservo1;
Adafruit_TiCoServo myservo2;
int openVal_1 = 180;
int closeVal_1 = 0;
int openVal_2 = 0;
int closeVal_2 = 180;
int expectedVal_1 = openVal_1;
int expectedVal_2 = openVal_2;
// the debounce time, increase if the output flickers

boolean readSensor() {
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();
  }
  previous = reading;
  if (state == HIGH) {
    return true;
  }
  else return false;
}

void setup()
{

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  // clock_prescale_set(clock_div_1);
  //#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP


  pinMode(inPin, INPUT);
  myservo1.attach(servo_pin1);
  myservo2.attach(servo_pin2);
  myservo1.write(openVal_1);
  myservo2.write(openVal_2);

}

void setColor (int r, int g, int b) {
  strip.setBrightness (128);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
    delay(0);
  }
  strip.show();
  return;
}

void setColor (int r, int g, int b, int brightness) {
  strip.setBrightness (brightness);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
    delay(0);
  }
  strip.show();
  return;
}

void fadeOut (int red, int green, int blue, int bright) { //26, 5, 0
  float brightness;

  for (int k = 255; k >= 0; k -= 4) {
    brightness = (k / 256.0) * bright;
    setColor((int)red, (int)green, (int)blue, (int)brightness);
    strip.show();
    if (!readSensor()) {
      return;
    }
  }
}

void loop()
{
  if (myservo1.read() > expectedVal_1) {
    pos_1--;
  }
  if (myservo2.read() > expectedVal_2) {
    pos_2--;
  }
  if (myservo1.read() < expectedVal_1) {
    pos_1++;
  }
  if (myservo2.read() < expectedVal_2) {
    pos_2++;
  }
  if (!readSensor()) {
    expectedVal_1 = openVal_1;
    expectedVal_2 = openVal_2;
  }
  else {
    expectedVal_1 = closeVal_1;
    expectedVal_2 = closeVal_2;
  }
  myservo1.write(pos_1);
  myservo2.write(pos_2);
  if (myservo1.read() == closeVal_1 && myservo2.read() == closeVal_2) {
    if (condition) {
      timer = millis();
      condition = false;
    }
    if (millis() - flashOn <= timer) {
      setColor(0, 0, 0);
    }
    if (millis() - flashOn > timer && millis() - flashOff <= timer) {
      setColor(255, 0, 0);
    }
    if (millis() - flashOff > timer) {
      if (con) {
        fadeOut(255, 0, 0, 128);
        con = false;
      }
      setColor(0, 0, 0);
    }
  }
  if (myservo1.read() == openVal_1 && myservo2.read() == openVal_2) {
    setColor(0, 0, 0);
    condition = true;
    con = true;
  }
}
