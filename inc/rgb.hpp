#ifndef __RGB_H__
#define __RGB_H__

#include "types.hpp"
#include "i2c.hpp"
#include "delay.hpp"


class RGB : 
    private I2C
{
public:
    enum RGB_color_t    { RGB_NONE = 0, RGB_WHITE, RGB_BLUE, RGB_GREEN, RGB_RED };
    enum RGB_color_id_t { RGB_BLUE_ID = 0x02, RGB_GREEN_ID = 0x03, RGB_RED_ID = 0x04 };
    enum RGB_blink_t    { RGB_BLINK_ON = 0, RGB_BLINK_OFF };
    enum RGB_bright_t   { RGB_BRIGHT_NONE = 0x00, RGB_BRIGHT_VERYLOW = 0x19,
                          RGB_BRIGHT_LOW = 0x32, RGB_BRIGHT_MEDIUM = 0x64, 
                          RGB_BRIGHT_HIGH = 0x96, RGB_BRIGHT_VERYHIGH = 0xC8, 
                          RGB_BRIGHT_FULL = 0xFF };
 
    RGB(const std::string& i2c_bus, uint8_t i2c_addr) :
                            I2C(i2c_bus, i2c_addr) { };

    void backlightBlink(RGB_blink_t state);
    void setBright(RGB_bright_t level);
    void setColor(RGB_color_t color);
    void setColor(uint8_t r_val, uint8_t g_val, uint8_t b_val);
    void setColor(RGB_color_id_t colorId, uint8_t pwmVal);

protected:
    void init();

private:
    void setMode1Reg(uint8_t value);
    void setMode2Reg(uint8_t value);
    void setDrvCtrlReg(uint8_t value);

    void send(uint8_t* data, uint8_t size);
};

#endif
