#include <unistd.h>
#include "delay.hpp"

typedef void (Delay::*delay_fn_t)(uint32_t) const;


int8_t Delay::wait(delay_t type, uint32_t value) const
{
    static const delay_fn_t delay_fn[] = 
    {
        [DELAY_UNDEF] = NULL,
        [DELAY_USEC]  = &Delay::delay_usec,
        [DELAY_MSEC]  = &Delay::delay_msec,
        [DELAY_SEC]   = &Delay::delay_sec,
    };
    static const uint8_t delay_fn_last = sizeof(delay_fn)/sizeof(*delay_fn); 
    int8_t ret = (-1);

    if(type < delay_fn_last && delay_fn[type] != NULL) {
        (this->*delay_fn[type])(value);
    }

    return ret;
}

void Delay::delay_sec(uint32_t value)  const
{ 
    sleep(value);
}

void Delay::delay_usec(uint32_t value) const
{
    usleep(value);
}

void Delay::delay_msec(uint32_t value) const
{
    uint32_t delaySec  = value / delay_max;
    uint32_t delayUsec = (value % delay_max) * 1000;

    delay_sec(delaySec);
    delay_usec(delayUsec);
}
