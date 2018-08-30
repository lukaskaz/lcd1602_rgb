#ifndef __DELAY_H__
#define __DELAY_H__

#include "types.hpp"


class Delay
{
public:
    enum delay_t { DELAY_UNDEF = 0, DELAY_USEC, DELAY_MSEC, DELAY_SEC };

    Delay() {};
    int8_t wait(delay_t type, uint32_t value) const;

private:
    static const uint32_t delay_max = 1000; 
    
    void delay_sec(uint32_t value)  const;
    void delay_usec(uint32_t value) const;
    void delay_msec(uint32_t value) const;
};

#endif
