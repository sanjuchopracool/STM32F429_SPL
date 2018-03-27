#ifndef SPIDISPLAY_H
#define SPIDISPLAY_H

#include "SPI5Driver.h"
#include "stdint.h"

class SPIDisplay : public SPI5Driver
{
public:
    SPIDisplay();
    void readID();

private:
    void writeReg( uint8_t inReg);
    void writeData(uint16_t inValue);
    void drawPixel(uint16_t x, uint16_t y, uint32_t color);
    void setCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
};

#endif // SPIDISPLAY_H
