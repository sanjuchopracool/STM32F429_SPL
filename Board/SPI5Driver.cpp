#include "SPI5Driver.h"

extern "C"
{
#include <stm32f4xx_spi.h>
}

// mosi PF9
// miso PF8
// sck  PF7

#define DRIVER_SPI                         SPI5
#define DRIVER_SPI_CLK                     RCC_APB2Periph_SPI5
#define DRIVER_SPI_CLK_INIT                RCC_APB2PeriphClockCmd

#define DRIVER_MOSI_PIN                     GPIO_Pin_9
#define DRIVER_MOSI_PORT                    GPIOF
#define DRIVER_MOSI_CLK                     RCC_AHB1Periph_GPIOF
#define DRIVER_MOSI_AF                      GPIO_AF_SPI5
#define DRIVER_MOSI_PIN_SOURCE              GPIO_PinSource9

#define DRIVER_MISO_PIN                     GPIO_Pin_8
#define DRIVER_MISO_PORT                    GPIOF
#define DRIVER_MISO_CLK                     RCC_AHB1Periph_GPIOF
#define DRIVER_MISO_AF                      GPIO_AF_SPI5
#define DRIVER_MISO_PIN_SOURCE              GPIO_PinSource8

#define DRIVER_SCK_PIN                     GPIO_Pin_7
#define DRIVER_SCK_PORT                    GPIOF
#define DRIVER_SCK_CLK                     RCC_AHB1Periph_GPIOF
#define DRIVER_SCK_AF                      GPIO_AF_SPI5
#define DRIVER_SCK_PIN_SOURCE              GPIO_PinSource7

SPI5Driver::SPI5Driver()
{
    // init gpio clocks
    RCC_AHB1PeriphClockCmd(DRIVER_MISO_CLK | DRIVER_MOSI_CLK |
                           DRIVER_SCK_CLK , ENABLE);
    // init rcc clocks
    DRIVER_SPI_CLK_INIT(DRIVER_SPI_CLK, ENABLE);

    //init alternate config for spi (mosi, SCK)
    GPIO_PinAFConfig(DRIVER_MOSI_PORT, DRIVER_MOSI_PIN_SOURCE, DRIVER_MOSI_AF);
    GPIO_PinAFConfig(DRIVER_MISO_PORT, DRIVER_MISO_PIN_SOURCE, DRIVER_MISO_AF);
    GPIO_PinAFConfig(DRIVER_SCK_PORT, DRIVER_SCK_PIN_SOURCE, DRIVER_SCK_AF);

    GPIO_InitTypeDef gpioConfig;
    gpioConfig.GPIO_Mode = GPIO_Mode_AF;
    gpioConfig.GPIO_OType = GPIO_OType_PP;
    gpioConfig.GPIO_Speed = GPIO_Speed_25MHz;
    gpioConfig.GPIO_PuPd = GPIO_PuPd_DOWN;


    // Mosi
    gpioConfig.GPIO_Pin = DRIVER_MOSI_PIN;
    GPIO_Init(DRIVER_MOSI_PORT, &gpioConfig);

    // Miso
    gpioConfig.GPIO_Pin = DRIVER_MISO_PIN;
    GPIO_Init(DRIVER_MISO_PORT, &gpioConfig);

    // SCK
    gpioConfig.GPIO_Pin = DRIVER_SCK_PIN;
    GPIO_Init(DRIVER_SCK_PORT, &gpioConfig);

    // SPI
    SPI_InitTypeDef  spiConfig;
    spiConfig.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spiConfig.SPI_Mode = SPI_Mode_Master;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_CPOL = SPI_CPOL_Low;
    spiConfig.SPI_CPHA = SPI_CPHA_1Edge;
    spiConfig.SPI_NSS = SPI_NSS_Soft;
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_CRCPolynomial = 7;
    SPI_Init(DRIVER_SPI, &spiConfig);

    SPI_Cmd(DRIVER_SPI, ENABLE);
}

uint16_t SPI5Driver::writeRead(uint16_t inByte )
{
    while(RESET == (DRIVER_SPI->SR & SPI_I2S_FLAG_TXE));
    SPI_SendData(DRIVER_SPI, inByte);
    while(RESET == (DRIVER_SPI->SR & SPI_I2S_FLAG_RXNE));
    return SPI_ReceiveData(SPI5);
}
