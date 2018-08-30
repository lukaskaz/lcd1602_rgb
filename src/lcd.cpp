#include "lcd.hpp"


/*******************************public*******************************/
return_t LCD::getDisplaySet(display_t selection, uint8_t& set)
{
    return_t ret = RET_SUCCESS;
 
    switch(selection) {
        case DISPLAY_ON:
            set |= DISP_CTRL_DISP_ON;
            break;
        case DISPLAY_OFF:
            set &= (uint8_t)(~DISP_CTRL_DISP_ON);
            break;
        case CURSOR_SHOW:
            set |= DISP_CTRL_CURS_ON;
            break;
        case CURSOR_HIDE:
            set &= (uint8_t)(~DISP_CTRL_CURS_ON);
            break;
        case CURSOR_BLINK:
            set |= DISP_CTRL_BLINK_ON;
            break;
        case CURSOR_NOBLINK:
            set &= (uint8_t)(~DISP_CTRL_BLINK_ON);
            break;
        default:
            // unsupported case, report failure
            ret = RET_FAIL;
            break;
    }

    return ret;
}

return_t LCD::display(display_t selection)
{
    return_t ret = RET_SUCCESS;
    uint8_t set = dispCtrlStatus;
 
    if(getDisplaySet(selection, set) == RET_SUCCESS) {
        setDisplayCtrl(set);
    }

    return ret;
}

return_t LCD::getScrollSet(scroll_t selection, uint8_t& set)
{
    return_t ret = RET_SUCCESS;
 
    switch(selection) {
        case SCROLL_CURS_LEFT:
            set = CURS_DISP_MVCURS | CURS_DISP_MVLEFT;
            break;
        case SCROLL_CURS_RIGHT:
            set = CURS_DISP_MVCURS | CURS_DISP_MVRIGHT;
            break;
        case SCROLL_TEXT_LEFT:
            set = CURS_DISP_MVDISP | CURS_DISP_MVLEFT;
            break;
        case SCROLL_TEXT_RIGHT:
            set = CURS_DISP_MVDISP | CURS_DISP_MVRIGHT;
            break;
        default:
            // unsupported case, report failure
            ret = RET_FAIL;
            break;
    }

    return ret;
}

return_t LCD::scroll(scroll_t selection, uint32_t moveNb, uint32_t moveDelayMs)
{
    return_t ret = RET_FAIL;
    uint8_t set = 0;

    if(getScrollSet(selection, set) == RET_SUCCESS) {
        for(uint32_t i = 0; i < moveNb; i++) {
            setCursDispShift(set);
            wait(DELAY_MSEC, moveDelayMs);
        }

        ret = RET_SUCCESS;
    }

    return ret;
}

return_t LCD::getTextEntrySet(textEntry_t selection, uint8_t& set)
{
    return_t ret = RET_SUCCESS;
 
    switch(selection) {
        case ENTRY_LEFTTORIGHT:
            set |= ENTRY_MODE_CURS_POS_INCR;
            break;
        case ENTRY_RIGHTTOLEFT:
            set &= (uint8_t)(~ENTRY_MODE_CURS_POS_INCR);
            break;
        case ENTRY_AUTOSCROLL:
            set |= ENTRY_MODE_DISP_SHIFT;
            break;
        case ENTRY_NOAUTOSCROLL:
            set &= (uint8_t)(~ENTRY_MODE_DISP_SHIFT);
            break;
        default:
            // unsupported case, report failure
            ret = RET_FAIL;
            break;
    }

    return ret;
}

return_t LCD::textEntry(textEntry_t selection)
{
    return_t ret = RET_SUCCESS;
    uint8_t set = entryModeSetState;
 
    if(getTextEntrySet(selection, set) == RET_SUCCESS) {
        setEntryModeSet(set);
    }

    return ret;
}

void LCD::setCustomSymbol(uint8_t location, uint8_t* charmap)
{
    location &= 0x7; // we only have 8 locations 0-7
    instruction(INSTR_SETCGRAMADDR | (location << 3));

    uint8_t data[9] = {0};
    data[0] = INSTR_CTRL_RS_DATA_REG | INSTR_CTRL_SINGLE_CMD;

    for(int i=0; i < 8; i++) {
        data[i+1] = charmap[i];
    }

    send(data, sizeof(data));
}

void LCD::setCursor(uint8_t row, uint8_t column)
{
    if(row < _rows && column < _cols) {
        uint8_t pos = (row == 0) ? CURS_LINE_1 : CURS_LINE_2;

        pos |= column;
        instruction(pos);
    }
}

size_t LCD::writeRAM(std::vector<uint8_t>& data)
{
    uint8_t ctrlByte = INSTR_CTRL_RS_DATA_REG | INSTR_CTRL_SINGLE_CMD;

    data.insert(data.begin(), ctrlByte);
    send(&data[0], data.size());
    return 1; // assume sucess
}

inline void LCD::instruction(uint8_t value)
{
    uint8_t ctrlByte = INSTR_CTRL_RS_INSTR_REG | INSTR_CTRL_SINGLE_CMD;
    uint8_t data[] = { ctrlByte, value };
    send(data, sizeof(data));
}

void LCD::print(const std::string& str)
{
    std::vector<uint8_t> data(str.begin(), str.end());
    writeRAM(data);
}

typedef enum {
    CMD_GROUP_UNDEF = 0,
    CMD_GROUP_DISPLAY_CLEAR,
    CMD_GROUP_MODE_SET,
    CMD_GROUP_DISPLAY_CTRL,
    CMD_GROUP_DISPLAY_SHIFT,
    CMD_GROUP_FUNCTION_SET   
} LCD_Cmd_Group_t;

int LCD::setFunctionSet(uint8_t set)
{
    int ret = (-1);
    uint8_t cmd = INSTR_FUNCSET | set;

    funcSetState = set;
    instruction(cmd);
    wait(DELAY_USEC, 40);  //Execution time ~40us

    ret = 0;
    return ret;
}

int LCD::setDisplayCtrl(uint8_t set)
{
    int ret = (-1);

    dispCtrlStatus = INSTR_DISPCONTROL | set;
    instruction(dispCtrlStatus);
    wait(DELAY_USEC, 40);   //Execution time ~40us

    ret = 0;
    return ret;
}

int LCD::setEntryModeSet(uint8_t set)
{
    int ret = (-1);
    uint8_t cmd = INSTR_ENTRYMODESET | set;

    entryModeSetState = set;
    instruction(cmd);
    wait(DELAY_USEC, 40);   //Execution time ~40us

    ret = 0;
    return ret;
}

return_t LCD::setCursDispShift(uint8_t set)
{
    return_t ret = RET_FAIL;
    uint8_t cmd = INSTR_CURSDISPSHIFT | set;

    instruction(cmd);
    wait(DELAY_USEC, 40);   //Execution time ~40us

    ret = RET_SUCCESS;
    return ret;
}

int LCD::clear(void)
{
    int ret = (-1);
    uint8_t cmd = INSTR_CLR_DISPLAY;

    instruction(cmd);
    wait(DELAY_MSEC, 2);   //Execution time ~2ms

    ret = 0;
    return ret;
}

int LCD::home(void)
{
    int ret = (-1);
    uint8_t cmd = INST_RET_HOME;

    instruction(cmd);
    wait(DELAY_MSEC, 2);   //Execution time ~2ms

    ret = 0;
    return ret;
}


/*******************************private*******************************/
void LCD::init(void) 
{
    // When LCD is powered up it executes a reset routine which lasts up to 50ms 
    // there must be a delay before initialization when LCD is powered up
    // there have to be a wait time up to 50ms before commands can be send to LCD
    wait(DELAY_MSEC, 50);

    // go on with proper LCD initialization
    setFunctionSet(FUNC_SET_5x8DOTS | FUNC_SET_2LINES | FUNC_SET_8BIT_MODE);
    setDisplayCtrl(DISP_CTRL_BLINK_OFF | DISP_CTRL_CURS_OFF | DISP_CTRL_DISP_ON);

    clear();
    setEntryModeSet(ENTRY_MODE_DISP_NO_SHIFT | ENTRY_MODE_CURS_POS_INCR);
}

void LCD::send(uint8_t* data, uint8_t size) const
{
    if(i2c_transfer(REQ_WRITE, data, size)) {
        log(LOG_ERROR, __func__, "Failed to transfer data to LCD driver");
        exit(4);
    }
}
