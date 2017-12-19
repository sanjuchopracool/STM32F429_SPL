#ifndef DISPLAYSPI_H
#define DISPLAYSPI_H

#include <cstdint>

class DisplaySPI
{
public:
    DisplaySPI();

    void SendData(uint8_t data);
    void SendCommand(uint8_t data);
    void fill( uint16_t x0, uint16_t y0,
               uint16_t x1, uint16_t y1, uint16_t color);

};

#endif // DISPLAYSPI_H
