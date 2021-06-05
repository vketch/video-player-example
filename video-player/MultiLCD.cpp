#include "MultiLCD.h"
#include <algorithm>

using namespace std;

MultiLCD::MultiLCD(MultiLCD(PinName mosi, PinName miso, PinName sclk, PinName reset, PinName DC, lcds_cs_pins &sc_pins){


    _lsds[sTop] = new LCDContext( new ILI9341V(SPI_DMA_, 10000000, mosi, miso, sclk, sc_pins.top, reset, DC, "myLCD"),  sc_pins.top);
    _lsds[sTop] = new LCDContext( new ILI9341V(SPI_DMA_, 10000000, mosi, miso, sclk, sc_pins.top, reset, DC, "myLCD"),  sc_pins.top);

    _contexts[sTop].    ILI9341V* lcd;

    _contexts[sTop]._cs = sc_pins.top;
    _contexts[sBottom]._cs = sc_pins.bottom;
    _contexts[sRight]._cs = sc_pins.right;
    _contexts[sLeft]._cs = sc_pins.left;
    _contexts[sFront]._cs = sc_pins.front;
    _contexts[sBack]._cs = sc_pins.back;
}

virtual ~MultiLCD(){
    for(lcd: _lcds)    
        lcd
};


void MultiLCD::set_active(ABCSide side){
    _curr_side = side;
}