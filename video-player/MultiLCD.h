
#ifndef MULTI_LCD_H
#define MULTI_LCD_H
#include <map>
#include "mbed.h"
#include "ILI9341V.h"


#define LCD_COUNT 6
enum ABCSide { sTop = 0, sBottom, sRight, sLeft, sFront, sBack, sUndefined };

struct lcds_cs_pins{
    ABCSide top;
    ABCSide bottom;
    ABCSide right;
    ABCSide left;
    ABCSide front;
    ABCSide back;
};

struct LCDContext{
    LCDContext(ILI9341V* display, PinName cs): _display(display), _cs(cs) {}
    ILI9341V* _display;
    PinName _cs; // Chip Select PIN
};

class MultiLCD{
public:
    MultiLCD(PinName mosi, PinName miso, PinName sclk, PinName reset, PinName DC, lcds_cs_pins &sc_pins);
    virtual ~MultiLCD();

    void set_active(ABCSide side);
    ABCSide get_active() const { return _curr_side; }
private:
    //LCDContext _contexts[LCD_COUNT];
    std::map<ABCSide, LCDContext> _displays;
    ABCSide _curr_side; 
};

#endif
