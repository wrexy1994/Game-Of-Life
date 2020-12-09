#ifndef _EFFECTS_H_
#define _EFFECTS_H_

// Based on a sketch found on the www.dfrobot.com website

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>

uint16_t lastColor = 0;
uint16_t lastPixel = 0;

// From Adafruit NeoMatrix
static uint16_t Color16(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

// From Adafruit NeoPixel
static uint32_t Color32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t wheel16(byte wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return Color16(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170)
  {
    wheelPos -= 85;
    return Color16(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return Color16(wheelPos * 3, 255 - wheelPos * 3, 0);
}

uint32_t wheel32(byte wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return Color32(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170)
  {
    wheelPos -= 85;
    return Color32(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return Color32(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void rainbowEffect(Adafruit_NeoMatrix *mat)
{
  int count = 0;
  // Draw diagonal lines, top left half
  for (int x = 0; x < BOARD_WIDTH; x++)
  {
    mat->drawLine(x, 0, 0, x, wheel16(((count++ * 64 / BOARD_HEIGHT) + lastColor) & 255));
  }
  // Draw diagonal lines, bottom left half
  for (int x = 1; x < BOARD_WIDTH; x++)
  {
    mat->drawLine(x, BOARD_HEIGHT - 1, BOARD_WIDTH - 1, x, wheel16(((count++ * 64 / BOARD_WIDTH) + lastColor) & 255));
  }
  lastColor++;
  if (lastColor >= 1280)
    lastColor = 0;
  mat->show();
}

void rainbow(Adafruit_NeoPixel *pixel_ptr)
{
  for (uint16_t i = 0; i < pixel_ptr->numPixels(); i++)
  {
    pixel_ptr->setPixelColor(i, wheel32((i + lastColor) & 255));
  }
  pixel_ptr->show();
  lastColor++;
  if (lastColor > 255)
      lastColor = 0;
}

void rainbowCycle(Adafruit_NeoPixel *pixel_ptr)
{
  for (uint16_t i = 0; i < pixel_ptr->numPixels(); i++)
  {
    pixel_ptr->setPixelColor(i, wheel32(((i * 256 / pixel_ptr->numPixels()) + lastColor) & 255));
  }
  pixel_ptr->show();
  lastColor++;
  if (lastColor > 1279)
      lastColor = 0;
}

void colorWipe(Adafruit_NeoPixel *pixel_ptr)
{
  static uint32_t c = pixel_ptr->gamma32(pixel_ptr->ColorHSV(random() % 65535L));
  pixel_ptr->setPixelColor(lastPixel, c);
  pixel_ptr->show();
  lastPixel++;
  if (lastPixel > pixel_ptr->numPixels())
  {
    lastPixel = 0;
    c = pixel_ptr->gamma32(pixel_ptr->ColorHSV(random() % 65535L));
  }
}

void theaterChase(Adafruit_NeoPixel *pixel_ptr)
{
  static uint32_t c = pixel_ptr->gamma32(pixel_ptr->ColorHSV(random() % 65535L));
  static uint8_t q = 0;
  static bool flip = true;
  if (flip)
  {
    for (int i = 0; i < pixel_ptr->numPixels(); i += 3)
    {
      pixel_ptr->setPixelColor(i + q, c);
    }
  }
  else
  {
    for (int i = 0; i < pixel_ptr->numPixels(); i += 3)
    {
      pixel_ptr->setPixelColor(i + q, 0);
    }
  }
  pixel_ptr->show();
  if (!flip)
  {
    q++;
    if (q > 2)
    {
      q = 0;
     lastPixel++;
     if (lastPixel > 9)
      {
        lastPixel = 0;
        c = pixel_ptr->gamma32(pixel_ptr->ColorHSV(random() % 65535L));
      }
    }
  }
  flip = !flip;
}

void theaterChaseRainbow(Adafruit_NeoPixel *pixel_ptr)
{
  static uint8_t q = 0;
  static bool flip = true;
  if (flip)
  {
    for (int i = 0; i < pixel_ptr->numPixels(); i += 3)
    {
      pixel_ptr->setPixelColor(i + q, wheel32((i + lastColor) % 255));
    }
  }
  else
  {
    for (int i = 0; i < pixel_ptr->numPixels(); i += 3)
    {
      pixel_ptr->setPixelColor(i + q, 0);
    }
  }
  pixel_ptr->show();
  if (!flip)
  {
    q++;
    if (q > 2)
    {
      q = 0;
     lastColor++;
     if (lastColor > 255)
      {
        lastColor = 0;
      }
    }
  }
  flip = !flip;
}

#endif
