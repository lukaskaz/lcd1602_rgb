#include "rgb.hpp"

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0
#define REG_PWM_FIRST   REG_BLUE

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_GRPPWM      0x06
#define REG_GRPFREQ     0x07
#define REG_LEDOUTPUT   0x08
#define REG_ADDR_INCR   0x80

#define RGB_NORMAL_PWR_MODE     0x00
#define RGB_PWM_BLINKING        0x20
#define RGB_PWM_DIMMING         0x00
#define RGB_CTRL_PWMX_GRPPWM    0xFF


void RGB::setColor(uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
    uint8_t data[] = { REG_BLUE | REG_ADDR_INCR , b_val, g_val, r_val };
    send(data, sizeof(data));
}

void RGB::setColor(RGB_color_id_t colorId, uint8_t pwmVal)
{
    uint8_t data[] = { colorId, pwmVal };
    send(data, sizeof(data));
}

void RGB::setMode1Reg(uint8_t value)
{
    uint8_t data[] = { REG_MODE1, value };
    send(data, sizeof(data));
}

void RGB::setMode2Reg(uint8_t value)
{
    uint8_t data[] = { REG_MODE2, value };
    send(data, sizeof(data));
}

void RGB::setDrvCtrlReg(uint8_t value)
{
    uint8_t data[] = { REG_LEDOUTPUT, value };
    send(data, sizeof(data));
}

void RGB::backlightBlink(RGB_blink_t state)
{
    uint8_t data[] = { REG_GRPPWM | REG_ADDR_INCR, 0xFF, 0x00 };

    switch(state) {
        case RGB_BLINK_ON:
            data[1] = 0x7F;
            data[2] = 0x17;
            break;
        case RGB_BLINK_OFF:
            data[1] = 0xFF;
            data[2] = 0x00;           
            break;
        default:
            // unsupported case, do nothing
            break;
    }

    setMode2Reg(RGB_PWM_BLINKING);
    send(data, sizeof(data));
}

void RGB::setBright(RGB_bright_t level)
{
    uint8_t data[] = { REG_GRPPWM , level };

    setMode2Reg(RGB_PWM_DIMMING);
    send(data, sizeof(data));
}

void RGB::setColor(RGB_color_t color)
{
    static const uint8_t rgb_color[][3] = 
    {
        [RGB_NONE] =  { 0,   0,   0   },
        [RGB_WHITE] = { 255, 255, 255 },
        [RGB_BLUE]  = { 0  , 0  , 255 },
        [RGB_GREEN] = { 0  , 255, 0   },
        [RGB_RED]   = { 255, 0  , 0   },
    };
    static const uint8_t rgb_color_last = sizeof(rgb_color)/sizeof(*rgb_color);

    if(color < rgb_color_last ) {
        uint8_t data[] =
        {
            REG_PWM_FIRST | REG_ADDR_INCR,
            rgb_color[color][2],
            rgb_color[color][1],
            rgb_color[color][0]
        };

        send(data, sizeof(data));
    }
}

/*******************************private*******************************/
void RGB::init(void) 
{
    // go on with proper backlight initialization
    setMode1Reg(RGB_NORMAL_PWR_MODE);
    setMode2Reg(RGB_PWM_BLINKING); // set MODE2: 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
    setDrvCtrlReg(RGB_CTRL_PWMX_GRPPWM); // set backlight controlled by both PWM and GRPPWM registers
    backlightBlink(RGB_BLINK_OFF);
    setBright(RGB_BRIGHT_FULL);

    setColor(RGB_WHITE);
}

void RGB::send(uint8_t* data, uint8_t size)
{
    if(i2c_transfer(REQ_WRITE, data, size)) {
        log(LOG_ERROR, __func__, "Failed to transfer data to RGB backlight driver");
        exit(4);
    }
}

