#ifndef BUFFER
#define BUFFER

#include "cgif.h"
#include "conway.h"

// need a double-buffered-grid class -- one buffer is read to -- one buffer is
// written to

namespace buffer {

std::default_random_engine rng;
std::uniform_int_distribution<int> dist {0, 26};
std::mutex mtx;

class double_buf {
public:
    const int width;
    const int height;
    const int wscale;
    const int hscale;
    int count;

    double_buf(int width, int height)
        : width {width}
        , height {height}
        , wscale {GIF_WIDTH / width}
        , hscale {GIF_HEIGHT / height}
        , count {0}
        , a {new bool[width * height]}
        , b {new bool[width * height]}
    {
        // put random things in a
        for (int i = 0; i < width * height; ++i) {
            if (dist(rng) == 0) {
                a[i] = 1;
                ++count; // add count here
            } else {
                a[i] = 0;
            }
        }
    }

    // does this behave correctly?
    const bool &operator()(int x, int y) const // read from first buffer
    {
        return first ? a[x + y * width] : b[x + y * width];
    }

    bool &operator()(int x, int y)
    {
        return first ? b[x + y * width] : a[x + y * width]; // write to buffer
    }

    void swap() { first = !first; }

    grid copy_back(int first, int last)
    {
        auto new_grid = std::make_unique<bool[]>((last - first) * sizeof(bool));
        for (int i = first; i < last; ++i)
            new_grid[i] = first ? b[i] : a[i];
        return new_grid;
    }

    grid copy_back() { return NULL; }

    const bool &front(int x, int y) const // read from first buffer
    {
        return first ? a[x + y * width] : b[x + y * width];
    }

    bool &back(int x, int y)
    {
        if (first)
            return b[x + y * width];
        else
            return a[x + y * width];
    }

    const grid &front() const { return first ? a : b; }
    grid &back() { return first ? b : a; }

    // each thread has a local copy that is the right size for the number of
    // rows it is responsible for
    void generate(grid &copy, const int id, const int nthreads)
    {
        int x, y, i, i_start, i_stop, stride, neighbors, count;
        bool alive;
        stride  = height / nthreads;
        i_start = id * stride;
        i_stop  = (id < nthreads - 1) ? i_start + stride : height;
        count   = 0;

        for (y = i_start; y < i_stop; ++y) { // divide these rows among threads
            for (x = 0; x < width; ++x) {
                alive     = front(x, y);
                neighbors = 0;
                for (i = 0; i < 9; ++i) {
                    if (x - 1 + (i % 3) < 0 || x - 1 + (i % 3) >= width
                        || y - 1 + (i / 3) < 0 || y - 1 + (i / 3) >= height
                        || i == 4)
                        continue;
                    if (front(x - 1 + i % 3, y - 1 + i / 3))
                        ++neighbors;
                }

                // rules for the game of life -- add count here so we can keep
                // track
                if (alive && neighbors < 2 || alive && neighbors > 3) {
                    // back(x, y) = false;
                    // write to copy instead of back
                    copy[x + y * width] = false;
                    count--;
                } else if (!alive && neighbors >= 3) {
                    // back(x, y) = true;
                    copy[x + y * width] = true;
                    count++;
                } else {
                    // back(x, y) = alive; // count doesn't change
                    copy[x + y * width] = alive;
                }
            }
        }
        /* critical section */
        for (y = i_start; y < i_stop; ++y) {
            for (x = 0; x < width; ++x) {
                back(x, y) = copy[x + y * width];
            }
        }
        mtx.lock();
        this->count += count;
        mtx.unlock();
    }

    // pass in gif pointer and frame object,
    // read from the front buffer.
    void snap(const cgif::t_cgif gif) const
    {
        for (int y = 0; y < hscale * height; ++y) {
            for (int x = 0; x < wscale * width; ++x) {
                gif.pImageData[x + y * GIF_WIDTH]
                    = (uint8_t)(front(x / wscale, y / hscale)) ? YELLOW : BLACK;
            }
        }
        init_frame_config(gif.fConfig, gif.pImageData,
                          GIF_DELAY); // initialize the frame-configuration
        cgif_addframe(gif.pGIF, gif.fConfig);
    }

private:
    bool first = true;
    grid a;
    grid b;
};

} /* namespace buffer */

#endif /* BUFFER */
