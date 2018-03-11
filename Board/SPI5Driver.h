#ifndef SPI5DRIVER_H
#define SPI5DRIVER_H

#include <cstdint>

class SPI5Driver
{
public:
    SPI5Driver();
    uint16_t WriteRead(uint16_t inByte );
};

#endif // SPI5DRIVER_H
