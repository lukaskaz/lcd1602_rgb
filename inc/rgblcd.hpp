#ifndef __RGBLCD_H__
#define __RGBLCD_H__

#include "types.hpp"
#include "lcd.hpp"
#include "rgb.hpp"


// declare singleton class
class RGBLCD : 
    public LCD,
    public RGB
{
public:
    static RGBLCD& get_instance(void);

private:
    RGBLCD();
    ~RGBLCD();
    RGBLCD(RGBLCD const&) = delete;
    RGBLCD& operator=(RGBLCD const&) = delete;
};

#endif
