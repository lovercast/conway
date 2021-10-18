#include "../external/cgif/cgif.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Small helper functions to initialize GIF- and frame-configuration */
static void init_GIF_config(CGIF_Config *pConfig, char *path, uint16_t width,
                            uint16_t height, uint8_t *pPalette,
                            uint16_t numColors)
{
    memset(pConfig, 0, sizeof(CGIF_Config));
    pConfig->width                   = width;
    pConfig->height                  = height;
    pConfig->pGlobalPalette          = pPalette;
    pConfig->numGlobalPaletteEntries = numColors;
    pConfig->path                    = path;
    pConfig->attrFlags               = CGIF_ATTR_IS_ANIMATED;
}

void init_frame_config(CGIF_FrameConfig *pConfig, uint8_t *pImageData,
                       uint16_t delay)
{
    memset(pConfig, 0, sizeof(CGIF_FrameConfig));
    pConfig->delay      = delay;
    pConfig->pImageData = pImageData;
}

typedef struct {
    CGIF *pGIF;
    CGIF_FrameConfig *fConfig;
    uint8_t *pImageData;
} t_cgif;

/* Entry Point routine */
t_cgif do_cgif_setup(const int width, const int height)
{
    CGIF *pGIF;                // struct containing the GIF
    CGIF_Config gConfig;       // global configuration parameters for the GIF
    CGIF_FrameConfig *fConfig; // configuration parameters for a frame
    uint8_t *pImageData;       // image data (an array of color-indices)
    uint8_t aPalette[] = {0xFF, 0xFF, 0x00,  // yellow
                          0x00, 0x00, 0x00}; // black
                                             // 0x00, 0x00, 0xFF // blue
    uint8_t numColors = 2;  // number of colors in aPalette (up to 256 possible)
    int numFrames     = 12; // number of frames in the video

    // initialize the GIF-configuration and create a new GIF
    init_GIF_config(&gConfig, "conway.gif", width, height, aPalette, numColors);
    pGIF       = cgif_newgif(&gConfig);
    fConfig    = malloc(sizeof(CGIF_FrameConfig));
    pImageData = malloc(width * height);
    t_cgif ret = {pGIF, fConfig, pImageData};
    return ret;
}

void do_cgif_cleanup(t_cgif to_clean)
{
    cgif_close(to_clean.pGIF);
    free(to_clean.fConfig);
    free(to_clean.pImageData);
}
