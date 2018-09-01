#include <string.h>
#include <signal.h>

#include "rgblcd.hpp"

const int colorR = 0;
const int colorG = 255;
const int colorB = 255;

static char user_name[32+1] = {0};

void signal_handler(int signal_kind)
{
    Log console(false, false, true, false);

    console.log(Log::LOG_INFO, __func__, "Received signal: "
                                + numToString(NUM_HEX, signal_kind));
    if(signal_kind == SIGINT) {
        RGBLCD& lcd = RGBLCD::get_instance();
        Delay delay;

        lcd.setColor(RGBLCD::RGB_RED);
        lcd.setCursor(0, 0);
        lcd.print("  Logging out!  ");

        lcd.setCursor(1, 0);
        lcd.print("BYE BYE " + std::string(user_name));
        
        delay.wait(Delay::DELAY_SEC, 5);
        exit(0);

        lcd.setBright(RGBLCD::RGB_BRIGHT_FULL);
        lcd.clear();
        lcd.print("FULL");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_VERYHIGH);
        lcd.clear();
        lcd.print("VERYHIGH");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_HIGH);
        lcd.clear();
        lcd.print("HIGH");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_MEDIUM);
        lcd.clear();
        lcd.print("MEDIUM");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_LOW);
        lcd.clear();
        lcd.print("LOW");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_VERYLOW);
        lcd.clear();
        lcd.print("VERYLOW");
        delay.wait(Delay::DELAY_SEC, 2);
        lcd.setBright(RGBLCD::RGB_BRIGHT_NONE);
        lcd.clear();
        lcd.print("NONE");
        delay.wait(Delay::DELAY_SEC, 5);

        lcd.clear();
        lcd.setColor(RGBLCD::RGB_NONE);
    }

    exit(0);
}

int main(int argc, char* argv[])
{
    Log console;

    if(argc > 1) {
        for(int i = 0; argv[i] != '\0'; i++) {
            if(!strncmp(argv[i], "-u", 3)) {
                strncpy(user_name, argv[i+1], 32);
            }
            console.log("Parameter " + numToString(NUM_DEC, i) + " = " + argv[i]);
        }
    }

    signal(SIGINT, signal_handler);

    console.log("Given username: '" + std::string(user_name) + "'");

    RGBLCD& lcd = RGBLCD::get_instance(16, 2);
    Delay delay;

    delay.wait(Delay::DELAY_SEC, 1);
    lcd.setColor(colorR, colorG, colorB);

    lcd.setCursor(0, 0);
    lcd.print(std::string("Logged: ") + user_name);
    char dgr[3] = {0};
    dgr[0] = 0xDF;
    dgr[1] = 'C';
    lcd.print(dgr);  
    
    char ticking_time[16+1] = { 0 };
    int time = 0;
    
    while(1) {
        //goto A1;
        snprintf(ticking_time, sizeof(ticking_time), "App time: %d", ++time);
        lcd.setCursor(1, 0);
        lcd.print(ticking_time);

        
        delay.wait(Delay::DELAY_SEC, 1);
        lcd.display(RGBLCD::DISPLAY_OFF);
        delay.wait(Delay::DELAY_SEC, 1);
        lcd.display(RGBLCD::DISPLAY_ON);
        
        lcd.display(RGBLCD::CURSOR_SHOW);
        delay.wait(Delay::DELAY_SEC, 1);
        lcd.display(RGBLCD::CURSOR_BLINK);
        delay.wait(Delay::DELAY_SEC, 3);
        lcd.display(RGBLCD::CURSOR_NOBLINK);
        delay.wait(Delay::DELAY_SEC, 1);
        lcd.display(RGBLCD::CURSOR_HIDE);
        delay.wait(Delay::DELAY_SEC, 1);

        lcd.scroll(RGBLCD::SCROLL_TEXT_LEFT, 16, 500);
        lcd.scroll(RGBLCD::SCROLL_TEXT_RIGHT, 16, 500);
        
        lcd.display(RGBLCD::CURSOR_SHOW);
        lcd.scroll(RGBLCD::SCROLL_CURS_LEFT, 5, 500);
        lcd.scroll(RGBLCD::SCROLL_CURS_RIGHT, 7, 500);
        lcd.display(RGBLCD::CURSOR_HIDE);

        lcd.clear();
        lcd.textEntry(RGBLCD::ENTRY_LEFTTORIGHT);
        lcd.setCursor(0, 0);
        lcd.print("Hello!:)");
        delay.wait(Delay::DELAY_SEC, 3);
        
        lcd.clear();
        lcd.textEntry(RGBLCD::ENTRY_RIGHTTOLEFT);
        lcd.setCursor(0, 7);
        lcd.print("Hello!:)");
        delay.wait(Delay::DELAY_SEC, 3);
        
        lcd.clear();
        lcd.textEntry(RGBLCD::ENTRY_LEFTTORIGHT);
        lcd.textEntry(RGBLCD::ENTRY_AUTOSCROLL);
        lcd.setCursor(0, 6);
        lcd.print("Hello!");
        delay.wait(Delay::DELAY_SEC, 5);
        
        lcd.clear();
        lcd.textEntry(RGBLCD::ENTRY_RIGHTTOLEFT);
        lcd.textEntry(RGBLCD::ENTRY_AUTOSCROLL);
        lcd.setCursor(1, 2);
        lcd.print("Hello!");
        delay.wait(Delay::DELAY_SEC, 5);
        while(1);

    }

    delay.wait(Delay::DELAY_SEC, 2);
    return 0;



    return 0;
}

