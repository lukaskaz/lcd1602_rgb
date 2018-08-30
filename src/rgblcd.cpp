#include "rgblcd.hpp"

/*!
 *  @brief Device I2C Address
 */

#define LCD_I2C_BUS     "/dev/i2c-1"
#define RGB_I2C_BUS     "/dev/i2c-1"

#define LCD_I2C_ADDRESS (0x7C>>1)
#define RGB_I2C_ADDRESS (0xC0>>1)


/*******************************public*******************************/
RGBLCD::RGBLCD():
    LCD(LCD_I2C_BUS, LCD_I2C_ADDRESS),
    RGB(RGB_I2C_BUS, RGB_I2C_ADDRESS)
{
    LCD::init();
    RGB::init();
}

RGBLCD::~RGBLCD()
{
    LCD::clear();
    setColor(RGB_NONE);
}

/*******************************private*******************************/
