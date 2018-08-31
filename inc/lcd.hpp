#ifndef __LCD_H__
#define __LCD_H__

#include <vector>

#include "types.hpp"
#include "i2c.hpp"
#include "delay.hpp"


class LCD : 
    private I2C,
    private Delay
{
public:
    LCD(const std::string& i2c_bus, uint8_t i2c_addr) :
                        I2C(i2c_bus, i2c_addr), Delay() { clearVariables(); };

    enum scroll_t    { SCROLL_CURS_LEFT = 0, SCROLL_CURS_RIGHT,
                       SCROLL_TEXT_LEFT,     SCROLL_TEXT_RIGHT };
                       
    enum textEntry_t { ENTRY_LEFTTORIGHT = 0, ENTRY_RIGHTTOLEFT,
                       ENTRY_AUTOSCROLL,      ENTRY_NOAUTOSCROLL };

    enum display_t   { DISPLAY_ON = 0, DISPLAY_OFF, CURSOR_SHOW,
                       CURSOR_HIDE, CURSOR_BLINK, CURSOR_NOBLINK }; 

    return_t display(display_t selection);
    return_t scroll(scroll_t selection, uint32_t moveNb, uint32_t moveDelayMs);
    return_t textEntry(textEntry_t selection);

    void setCustomSymbol(uint8_t, uint8_t*);
    void setCursor(uint8_t, uint8_t);  

    void print(const std::string& str);

    int clear(void);
    int home(void);

    void setDispSize(uint8_t colsNb, uint8_t rowsNb)
                            { columns = colsNb; rows = rowsNb; }

protected:
    void init();
    bool isDispSizeInitialized(void)
            { if( columns != 0 && rows != 0) return true; return false; }

private:
    enum instr_t     { INSTR_CLR_DISPLAY   = 0x01, INST_RET_HOME      = 0x02,
                       INSTR_ENTRYMODESET  = 0x04, INSTR_DISPCONTROL  = 0x08,
                       INSTR_CURSDISPSHIFT = 0x10, INSTR_FUNCSET      = 0x20,
                       INSTR_SETCGRAMADDR  = 0x40, INSTR_SETDDRAMADDR = 0x80 };

    enum instrCtrl_t { INSTR_CTRL_SINGLE_CMD   = 0x00, INSTR_CTRL_MANY_CMDS   = 0x80,
                       INSTR_CTRL_RS_INSTR_REG = 0x00, INSTR_CTRL_RS_DATA_REG = 0x40 };

    enum funcSet_t   { FUNC_SET_5x8DOTS   = 0x00, FUNC_SET_5x10DOTS  = 0x04,
                       FUNC_SET_1LINE     = 0x00, FUNC_SET_2LINES    = 0x08, 
                       FUNC_SET_4BIT_MODE = 0x00, FUNC_SET_8BIT_MODE = 0x10 };

    enum dispCtrl_t  { DISP_CTRL_BLINK_OFF = 0x00, DISP_CTRL_BLINK_ON = 0x01,
                       DISP_CTRL_CURS_OFF  = 0x00, DISP_CTRL_CURS_ON  = 0x02,
                       DISP_CTRL_DISP_OFF  = 0x00, DISP_CTRL_DISP_ON  = 0x04 };

    enum entryModeSet_t 
                     { ENTRY_MODE_DISP_NO_SHIFT = 0x00, ENTRY_MODE_DISP_SHIFT    = 0x01,
                       ENTRY_MODE_CURS_POS_DECR = 0x00, ENTRY_MODE_CURS_POS_INCR = 0x02 };

    enum cursDispShift_t 
                     { CURS_DISP_MVLEFT = 0x00, CURS_DISP_MVRIGHT = 0x04, 
                       CURS_DISP_MVCURS = 0x00, CURS_DISP_MVDISP  = 0x08 };

    enum cursLine_t  { CURS_LINE_1 = 0x80, CURS_LINE_2 = 0xC0 };

    uint8_t funcSetState;
    uint8_t dispCtrlStatus;
    uint8_t entryModeSetState;

    uint8_t columns;
    uint8_t rows;
    
    void clearVariables(void) { funcSetState = 0; dispCtrlStatus = 0;
                        entryModeSetState = 0; columns = 0; rows = 0; }

    return_t setFunctionSet(uint8_t set);
    return_t setDisplayCtrl(uint8_t set);
    return_t setEntryModeSet(uint8_t set);
    return_t setCursDispShift(uint8_t set);

    return_t getDisplaySet(display_t selection, uint8_t& set);
    return_t getScrollSet(scroll_t selection, uint8_t& set);
    return_t getTextEntrySet(textEntry_t selection, uint8_t& set);

    size_t writeData(std::vector<uint8_t>& data);
    void writeInstr(uint8_t);

    void send(uint8_t* data, uint8_t size) const;
};

#endif
