#ifndef VARIOUS_TIMER_HPP
#define VARIOUS_TIMER_HPP

#include <SDL2/SDL.h>

class timer {
public:
    timer();

    void start();
    void stop();
    void pause();
    void unpause();

    Uint32 get_ticks();

    bool is_started();
    bool is_paused();

private:
    // The clock time when the timer started
    Uint32 start_ticks;

    // The ticks stored when the timer was paused
    Uint32 paused_ticks;

    // The timer status
    bool paused;
    bool started;
};

#endif // VARIOUS_TIMER_HPP