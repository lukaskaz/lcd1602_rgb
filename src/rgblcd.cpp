#include "rgblcd.hpp"

/*!
 *  @brief Device I2C Address
 */

#define LCD_I2C_BUS      "/dev/i2c-1"
#define RGB_I2C_BUS      "/dev/i2c-1"

#define LCD_I2C_ADDRESS  (0x7C>>1)
#define RGB_I2C_ADDRESS  (0xC0>>1)

#define LCD_DEFAULT_COLS 16
#define LCD_DEFAULT_ROWS 2

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
    clear();
    display(DISPLAY_OFF);

    setColor(RGB_NONE);
}

RGBLCD& RGBLCD::get_instance(void)
{
    static RGBLCD instance;

    if(instance.isDispSizeInitialized() == false) {
        instance.setDispSize(LCD_DEFAULT_COLS, LCD_DEFAULT_ROWS);
    }

    return instance;
}

RGBLCD& RGBLCD::get_instance(uint8_t cols, uint8_t rows)
{
    RGBLCD& instance = get_instance();
    instance.setDispSize(cols, rows);

    return instance;
}

/*******************************private*******************************/
