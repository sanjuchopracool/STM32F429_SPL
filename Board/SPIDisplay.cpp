#include "SPIDisplay.h"
#include <system.h>

extern "C"
{
#include <stm32f4xx_gpio.h>
}

#define CS_PIN                  GPIO_Pin_2
#define CS_PORT                 GPIOC
#define CS_CLK_ENABLE()         RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE )
#define CS_LOW()                GPIO_ResetBits( CS_PORT, CS_PIN )
#define CS_HIGH()               GPIO_SetBits( CS_PORT, CS_PIN )

#define RDX_PIN                  GPIO_Pin_12;
#define RDX_PORT                 GPIOD
#define RDX_CLK_ENABLE()         RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE )
#define RDX_LOW()                GPIO_ResetBits( RDX_PORT, RDX_PIN );
#define RDX_HIGH()               GPIO_SetBits( RDX_PORT, RDX_PIN );

#define WRX_PIN                  GPIO_Pin_13
#define WRX_PORT                 GPIOD
#define WRX_CLK_ENABLE()         RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE )
#define WRX_LOW()                GPIO_ResetBits( WRX_PORT, WRX_PIN )
#define WRX_HIGH()              GPIO_SetBits( WRX_PORT, WRX_PIN )


#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC	0xF7

SPIDisplay::SPIDisplay()
{
    CS_CLK_ENABLE();
    WRX_CLK_ENABLE();
    RDX_CLK_ENABLE();

    GPIO_InitTypeDef gpioConfig;
    gpioConfig.GPIO_Pin = CS_PIN;
    gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
    gpioConfig.GPIO_OType = GPIO_OType_PP;
    gpioConfig.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CS_PORT, &gpioConfig);

    gpioConfig.GPIO_Pin = WRX_PIN;
    GPIO_Init(WRX_PORT, &gpioConfig);

    gpioConfig.GPIO_Pin = RDX_PIN;
    GPIO_Init(RDX_PORT, &gpioConfig);

    CS_LOW();
    CS_HIGH();

    /* Software reset */
        writeReg(ILI9341_RESET);
        delay(10);

        writeReg(ILI9341_POWERA);
        writeData(0x39);
        writeData(0x2C);
        writeData(0x00);
        writeData(0x34);
        writeData(0x02);
        writeReg(ILI9341_POWERB);
        writeData(0x00);
        writeData(0xC1);
        writeData(0x30);
        writeReg(ILI9341_DTCA);
        writeData(0x85);
        writeData(0x00);
        writeData(0x78);
        writeReg(ILI9341_DTCB);
        writeData(0x00);
        writeData(0x00);
        writeReg(ILI9341_POWER_SEQ);
        writeData(0x64);
        writeData(0x03);
        writeData(0x12);
        writeData(0x81);
        writeReg(ILI9341_PRC);
        writeData(0x20);
        writeReg(ILI9341_POWER1);
        writeData(0x23);
        writeReg(ILI9341_POWER2);
        writeData(0x10);
        writeReg(ILI9341_VCOM1);
        writeData(0x3E);
        writeData(0x28);
        writeReg(ILI9341_VCOM2);
        writeData(0x86);
        writeReg(ILI9341_MAC);
        writeData(0x48);
        writeReg(ILI9341_PIXEL_FORMAT);
        writeData(0x55);
        writeReg(ILI9341_FRC);
        writeData(0x00);
        writeData(0x18);
        writeReg(ILI9341_DFC);
        writeData(0x08);
        writeData(0x82);
        writeData(0x27);
        writeReg(ILI9341_3GAMMA_EN);
        writeData(0x00);
        writeReg(ILI9341_COLUMN_ADDR);
        writeData(0x00);
        writeData(0x00);
        writeData(0x00);
        writeData(0xEF);
        writeReg(ILI9341_PAGE_ADDR);
        writeData(0x00);
        writeData(0x00);
        writeData(0x01);
        writeData(0x3F);
        writeReg(ILI9341_GAMMA);
        writeData(0x01);
        writeReg(ILI9341_PGAMMA);
        writeData(0x0F);
        writeData(0x31);
        writeData(0x2B);
        writeData(0x0C);
        writeData(0x0E);
        writeData(0x08);
        writeData(0x4E);
        writeData(0xF1);
        writeData(0x37);
        writeData(0x07);
        writeData(0x10);
        writeData(0x03);
        writeData(0x0E);
        writeData(0x09);
        writeData(0x00);
        writeReg(ILI9341_NGAMMA);
        writeData(0x00);
        writeData(0x0E);
        writeData(0x14);
        writeData(0x03);
        writeData(0x11);
        writeData(0x07);
        writeData(0x31);
        writeData(0xC1);
        writeData(0x48);
        writeData(0x08);
        writeData(0x0F);
        writeData(0x0C);
        writeData(0x31);
        writeData(0x36);
        writeData(0x0F);
        writeReg(ILI9341_SLEEP_OUT);

        delay(10);

        writeReg(ILI9341_DISPLAY_ON);
    writeReg(ILI9341_GRAM);
}

void SPIDisplay::writeReg(uint8_t inReg)
{
    WRX_LOW(); // write reg
    CS_LOW();
    writeRead( inReg );
    CS_HIGH();
}

void SPIDisplay::writeData(uint16_t inValue)
{
    WRX_HIGH(); // write data
    CS_LOW();
    writeRead( inValue );
    CS_HIGH();
}
void SPIDisplay::drawPixel(uint16_t x, uint16_t y, uint32_t color) {
    setCursorPosition(x, y, x, y);

    writeReg(ILI9341_GRAM);
    writeData(color >> 8);
    writeData(color & 0xFF);
}


void SPIDisplay::setCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    writeReg(ILI9341_COLUMN_ADDR);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);
    writeData(x2 >> 8);
    writeData(x2 & 0xFF);

    writeReg(ILI9341_PAGE_ADDR);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);
    writeData(y2 >> 8);
    writeData(y2 & 0xFF);
}

#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/**
 * @brief  SPI wait till end
 */
#define SPI_WAIT(SPIx)        while (SPI_IS_BUSY(SPIx))

void SPIDisplay::readID()
{
    setCursorPosition(0, 0, 239, 359);
    uint32_t pixelCount = 240*360;
    writeReg(ILI9341_GRAM);
    CS_LOW();
    WRX_HIGH(); // write data

    SPI_DataSizeConfig(SPI5,SPI_DataSize_16b);
    while(pixelCount--)
    {
        while(RESET == (SPI5->SR & SPI_I2S_FLAG_TXE));
        SPI_SendData( SPI5, 0XF800);
    }
    CS_HIGH();
    SPI_DataSizeConfig(SPI5,SPI_DataSize_8b);
}
