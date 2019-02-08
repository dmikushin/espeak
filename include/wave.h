#ifndef WAVE_H
#define WAVE_H

#ifndef PLATFORM_DOS
#include "stdint.h"
#endif

extern int32_t option_device_number;

extern int32_t wave_init(int32_t samplerate);
// TBD: the arg could be "alsa", "oss",...
extern void* wave_open(const char* the_api);

extern size_t wave_write(void* theHandler, char* theMono16BitsWaveBuffer, size_t theSize);
extern int32_t wave_close(void* theHandler);
extern void wave_flush(void* theHandler);
extern int32_t wave_is_busy(void* theHandler);
extern void wave_terminate();
extern uint32_t wave_get_read_position(void* theHandler);
extern uint32_t wave_get_write_position(void* theHandler);

// Supply the remaining time in ms before the sample is played
// (or 0 if the event has been already played).
// sample: sample identifier
// time: supplied value in ms
//
// return 0 if ok or -1 otherwise (stream not opened).
extern int32_t wave_get_remaining_time(uint32_t sample, uint32_t * time);

// set the callback which informs if the output is still enabled.
// Helpful if a new sample is waiting for free space whereas sound must be stopped.
typedef int32_t (t_wave_callback)(void);
extern void wave_set_callback_is_output_enabled(t_wave_callback* cb);


// general functions
extern void clock_gettime2(struct timespec *ts);
extern void add_time_in_ms(struct timespec *ts, int32_t time_in_ms);

// for tests
extern void *wave_test_get_write_buffer();

#endif
