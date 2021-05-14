#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include <stdint.h>

#define TIMER_HZ 50

void timer_init();

uint64_t timer_uptime();

/**
 * Returns the number of _ticks_ since the timer has been initialized,
 *
 * @return the number of _ticks_ since the timer has been initialized
 */
uint64_t timer_tick();

#endif