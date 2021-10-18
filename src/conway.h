#ifndef __CONWAY__ /* include guards */
#define __CONWAY__

#include <algorithm>
#include <array>
#include <atomic>
#include <barrier>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

using grid = std::unique_ptr<bool[]>;

#include "../external/cgif/cgif.h"

/* gif-encoding related constants */
enum { YELLOW = 0, BLACK = 1 };
#define GIF_WIDTH  512
#define GIF_HEIGHT 512
#define GIF_FRAMES 300
#define GIF_DELAY  10

namespace cgif {

struct t_cgif {
    CGIF *pGIF;
    CGIF_FrameConfig *fConfig;
    uint8_t *pImageData;
};

} /* namespace cgif */

/* functions */
extern "C" { // turns off C++ function mangling

extern void init_frame_config(CGIF_FrameConfig *pConfig, uint8_t *pImageData,
                              uint16_t delay);
extern cgif::t_cgif do_cgif_setup(const int width, const int height);
extern void do_cgif_cleanup(cgif::t_cgif to_clean);

} /* extern "C" */

#include "conway_grid.h"

void thread_fun(conway_grid::double_buf *db, int id, int nthreads, uint8_t &flag);
bool ready(void);

/* globals */
// std::atomic<int> flag; // limit number of threads to 32 bits TODO change this
// to accomodate more threads
// int ready;
bool need_frames = true;

#define flag_size sizeof(int) * 8

std::vector<uint8_t> flag;

std::atomic<int> goddamit;

#endif /* __CONWAY__ */
