class Timer {
public:
    u32 start_ticks;
    u32 paused_ticks;

    bool started;
    bool paused;

    Timer();
    void start();
    void stop();
    void pause();
    void unpause();

    u32 get_ticks();
};

Timer::Timer() {
    start_ticks = 0;
    paused_ticks = 0;
    started = false;
    paused = false;
}

void Timer::start() {
    started = true;
    paused = false;
    start_ticks = SDL_GetTicks();
    paused_ticks = 0;
}

void Timer::stop() {
    started = false;
    paused = false;
    start_ticks = 0;
    paused_ticks = 0;
}

void Timer::pause() {
    if (started && !paused) {
        paused = true;
        paused_ticks = SDL_GetTicks() - start_ticks;
        start_ticks = 0;
    }
}

void Timer::unpause() {
    if (started && paused) {
        paused = false;
        start_ticks = SDL_GetTicks() - paused_ticks;
        paused_ticks = 0;
    }
}

u32 Timer::get_ticks() {
    u32 time = 0;
    if (started) {
        if (paused) {
            time = paused_ticks;
        } else {
            time = SDL_GetTicks() - start_ticks;
        }
    }
    return time;
}
