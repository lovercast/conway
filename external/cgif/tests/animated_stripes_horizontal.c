#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cgif.h"

#define WIDTH  100
#define HEIGHT 100

/* This is an example code that creates a GIF-animation with a moving stripe-pattern. */
int main(void) {
  CGIF*          pGIF;
  CGIF_Config      gConfig;
  CGIF_FrameConfig    fConfig;
  uint8_t*      pImageData;
  uint8_t       aPalette[] = {
    0xFF, 0x00, 0x00,
    0xEE, 0x00, 0x00,
    0xDD, 0x00, 0x00,
    0xCC, 0x00, 0x00,
    0xBB, 0x00, 0x00,
    0xAA, 0x00, 0x00,
    0x99, 0x00, 0x00,
    0x88, 0x00, 0x00,
    0x77, 0x00, 0x00,
    0x66, 0x00, 0x00,
  };
  uint8_t numColors   = 10;  // number of colors in aPalette
  int numFrames       = 10; // number of frames in the video
  
  memset(&gConfig, 0, sizeof(CGIF_Config));
  memset(&fConfig, 0, sizeof(CGIF_FrameConfig));
  gConfig.attrFlags               = CGIF_ATTR_IS_ANIMATED; // set needed attribution flag (as GIF is animated) 
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.pGlobalPalette          = aPalette;
  gConfig.numGlobalPaletteEntries = numColors;
  gConfig.path                    = "animated_stripes_horizontal.gif";
  //
  // create new GIF
  pGIF = cgif_newgif(&gConfig);
  //
  // add frames to GIF
  pImageData = malloc(WIDTH * HEIGHT);         // actual image data
  fConfig.genFlags   = CGIF_FRAME_GEN_USE_DIFF_WINDOW | CGIF_FRAME_GEN_USE_TRANSPARENCY;
  fConfig.pImageData = pImageData;             // set pointer to image data
  fConfig.delay      = 50;                     // set time before next frame (in units of 0.01 s)
  for (int f = 0; f < numFrames; ++f) {
    for (int i = 0; i < (WIDTH * HEIGHT); ++i) {
    	pImageData[i] = (unsigned char)((f + numColors * i / WIDTH / HEIGHT) % numColors); // ceate a moving stripe pattern
    }
    cgif_addframe(pGIF, &fConfig); // append the new frame
  }
  free(pImageData);
  //
  // write GIF to file
  cgif_close(pGIF);                  // free allocated space at the end of the session
  return 0;
}
