#include "conway.h"

int main()
{
    int prob               = 3; // what was this supposed to do?
    constexpr int nthreads = 4;
    constexpr int width    = 256;
    constexpr int height   = 256; // TODO make this settable at runtime
    int nsnaps             = 0;   // snaps so far

    auto db = std::make_unique<buffer::double_buf>(
        buffer::double_buf(width, height)); // no new

    // create copy of bool array that is the right size
    grid copy        = std::make_unique<bool[]>(width * height * sizeof(bool));
    cgif::t_cgif gif = do_cgif_setup(GIF_WIDTH, GIF_HEIGHT);

    std::vector<std::thread> threads;

    std::barrier sync_point(
        nthreads + 1, [&]() { db->swap(); }); // always swap before release

    for (int i = 0; i < nthreads; ++i) {
        flag.push_back(0x00);
        threads.push_back(std::thread([&, i]() { // thread lambda
            int nrows;
            grid rows;

            nrows = db->width / nthreads
                  + (i < nthreads - 1) * db->width % nthreads;
            rows = std::make_unique<bool[]>(db->width * db->height
                                            * sizeof(bool));

            while (need_frames) {
                db->generate(rows, i, nthreads);
                sync_point.arrive_and_wait();
            }
        }));
    }

    while (nsnaps < GIF_FRAMES) {
        sync_point.arrive_and_wait();
        db->snap(gif);
        ++nsnaps;
    }

    need_frames = false;
    sync_point.arrive_and_wait();

    for (auto &th : threads) /* threads have to join or else libc++abi error */
        th.join();

    do_cgif_cleanup(gif);
    return 0;
}
