/**
*
* To create the images as C arrays, visit:
* https://notisrac.github.io/FileToCArray/
*
*/
#include <PNGdec.h>
#include <TFT_eSPI.h>

#include "animation_frames.h"
#include "background1.h"
#include "background2.h"
#include "background3.h"

#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"

// PNG decoder and TFT display instances
PNG png;
//TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft); // Off-screen buffer

#define MAX_IMAGE_WIDTH 240 

// Backgrounds with optional animation support
struct Background {
  const uint8_t * const *frames;
  const size_t *frameSizes;
  size_t frameCount;
};

const uint8_t * const background1Frames[] = {background1};
const size_t background1FrameSizes[] = {sizeof(background1)};

const uint8_t * const background2Frames[] = {background2};
const size_t background2FrameSizes[] = {sizeof(background2)};

const uint8_t * const background3Frames[] = {background3};
const size_t background3FrameSizes[] = {sizeof(background3)};

const Background backgrounds[] = {
    {animation_frames, animation_frame_sizes, animation_frame_count},
    {background1Frames, background1FrameSizes, 1},
    {background2Frames, background2FrameSizes, 1},
    {background3Frames, background3FrameSizes, 1},
};
const size_t numBackgrounds = sizeof(backgrounds) / sizeof(backgrounds[0]);

int currentBackground = 0; // Index of the current background
size_t currentFrame = 0;
unsigned long lastFrameTime = 0;
const uint16_t frameDelayMs = 40; // Frame time for animations (~25 FPS)

// Function to draw PNG to the sprite (callback for PNG decoder)
int pngDrawToSprite(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  sprite.pushImage(0, pDraw->y, pDraw->iWidth, 1, lineBuffer);
  return 1; // Continue decoding
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nUsing the PNGdec library with animation support");

  // Initialize TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  sprite.setColorDepth(16);
  sprite.setSwapBytes(true);
  sprite.createSprite(240, 240); // Match display size

  lastFrameTime = millis();

  Serial.println("Setup complete.");
}

void loop() {
  // Clear the sprite for the new frame
  sprite.fillScreen(TFT_BLACK);

  const Background &bg = backgrounds[currentBackground];

  if (bg.frameCount > 1) {
    unsigned long now = millis();
    if (now - lastFrameTime >= frameDelayMs) {
      currentFrame = (currentFrame + 1) % bg.frameCount;
      lastFrameTime = now;
    }
  } else {
    currentFrame = 0;
  }

  const uint8_t *frameData = bg.frames[currentFrame];
  size_t frameSize = bg.frameSizes[currentFrame];

  // Render the current background frame to the sprite
  int16_t rc = png.openFLASH((uint8_t *)frameData,
                           frameSize,
                           pngDrawToSprite);


  if (rc != PNG_SUCCESS) {
    Serial.println("Failed to open PNG file!");
    return;
  }
  png.decode(NULL, 0); // Decode and render background

  // Push the sprite to the display
  sprite.pushSprite(0, 0);

  // No touch input handling required in this build
}
