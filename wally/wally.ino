#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include "wally.h"
#inlcude "efects.h"

#define MINUTES 60 * 100
#define TIMER_GAME 5 * MINUTES
#define TIMER_TEMP 2 * MINUTES

#define DHT_PIN 9
#define LED_PIN 11
#define MAX_EFFECT 7

unsigned long last_change = 0;
bool show_game = true;
uint8_t show_effect = 0;
int display_slowness = 200;

Adafruit_NeoPixel pixels(BOARD_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(BOARD_WIDTH, BOARD_HEIGHT, LED_PIN,
                                                    NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                                                        NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                                    NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  pixels.setBrightness(100);
  pixels.begin();

  matrix->begin();
  add_pattern(PATTERN_ACORNS, 2, 2);
  last_change = millis();
}

void loop()
{
  if (show_game)
  {
    Serial.println("Displaying effects");
    switch (show_effect)
    {
    case 0:
      display_slowness = 200;
      display_game(&pixels);
      break;
    case 1:
      display_slowness = 20;
      rainbowEffect(matrix);
      break;
    case 2:
      display_slowness = 50;
      colorWipe(&pixels);
      break;
    case 3:
      display_slowness = 50;
      rainbow(&pixels);
      break;
    case 4:
      display_slowness = 50;
      rainbowCycle(&pixels);
      break;
    case 5:
      display_slowness = 50;
      theaterChase(&pixels);
      break;
    case 6:
      display_slowness = 50;
      theaterChaseRainbow(&pixels);
      break;
    }
    if ((millis() - last_change) > TIMER_GAME)
    {
      show_game = false;
      last_change = millis();
      show_effect++;
      show_effect %= MAX_EFFECT;
    }
  }
  else
  {
    display_slowness = 1000; // DHT updates once every second
    if ((millis() - last_change) > TIMER_TEMP)
    {
      show_game = true;
      last_change = millis();
    }
    Serial.println("Displaying Temp");
    display_temp(matrix, DHT_PIN);
  }
  delay(display_slowness);
}
