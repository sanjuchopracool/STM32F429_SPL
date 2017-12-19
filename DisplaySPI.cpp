#include "DisplaySPI.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#define DISPLAY_SPI                         SPI5
#define DISPLAY_SPI_CLK                     RCC_APB2Periph_SPI5
#define DISPLAY_SPI_CLK_INIT                RCC_APB2PeriphClockCmd

#define DISPLAY_CSX_PIN                     GPIO_Pin_2
#define DISPLAY_CSX_PORT                    GPIOC
#define DISPLAY_CSX_CLK                     RCC_AHB1Periph_GPIOC

#define DISPLAY_DCX_PIN                     GPIO_Pin_13
#define DISPLAY_DCX_PORT                    GPIOD
#define DISPLAY_DCX_CLK                     RCC_AHB1Periph_GPIOD

#define DISPLAY_SCL_PIN                     GPIO_Pin_7
#define DISPLAY_SCL_PORT                    GPIOF
#define DISPLAY_SCL_CLK                     RCC_AHB1Periph_GPIOF
#define DISPLAY_SCL_AF                      GPIO_AF_SPI5
#define DISPLAY_SCL_PIN_SOURCE              GPIO_PinSource7

#define DISPLAY_SDI_PIN                     GPIO_Pin_9
#define DISPLAY_SDI_PORT                    GPIOF
#define DISPLAY_SDI_CLK                     RCC_AHB1Periph_GPIOF
#define DISPLAY_SDI_AF                      GPIO_AF_SPI5
#define DISPLAY_SDI_PIN_SOURCE              GPIO_PinSource9

#define DISPLAY_CS_LOW()       GPIO_ResetBits(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN)
#define DISPLAY_CS_HIGH()      GPIO_SetBits(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN)
#define DISPLAY_DCX_SET()				GPIO_SetBits(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN)
#define DISPLAY_DCX_RESET()           GPIO_ResetBits(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN)

//  DISPLAY
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

DisplaySPI::DisplaySPI()
{
    // init gpio spi
    // RESET -> NRST
    // CSX -> PC2
    // DCX -> PD13
    // SCL -> PF7
    // SDI -> PF9 (mosi)


    // init gpio clocks
    RCC_AHB1PeriphClockCmd(DISPLAY_CSX_CLK | DISPLAY_DCX_CLK |
                           DISPLAY_SCL_CLK | DISPLAY_SDI_CLK, ENABLE);
    // init rcc clocks
    DISPLAY_SPI_CLK_INIT(DISPLAY_SPI_CLK, ENABLE);

    //init alternate config for spi (mosi, scl)
    GPIO_PinAFConfig(DISPLAY_SDI_PORT, DISPLAY_SDI_PIN_SOURCE, DISPLAY_SDI_AF);
    GPIO_PinAFConfig(DISPLAY_SCL_PORT, DISPLAY_SCL_PIN_SOURCE, DISPLAY_SCL_AF);

    GPIO_InitTypeDef gpioConfig;
    gpioConfig.GPIO_Mode = GPIO_Mode_AF;
    gpioConfig.GPIO_OType = GPIO_OType_PP;
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    gpioConfig.GPIO_PuPd = GPIO_PuPd_DOWN;

    // Mosi
    gpioConfig.GPIO_Pin = DISPLAY_SDI_PIN;
    GPIO_Init(DISPLAY_SDI_PORT, &gpioConfig);

    // SCL
    gpioConfig.GPIO_Pin = DISPLAY_SCL_PIN;
    GPIO_Init(DISPLAY_SCL_PORT, &gpioConfig);

    // CSX and DCX pin
    gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
    gpioConfig.GPIO_OType = GPIO_OType_PP;
    gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
    gpioConfig.GPIO_PuPd = GPIO_PuPd_NOPULL;

    gpioConfig.GPIO_Pin = DISPLAY_CSX_PIN;
    GPIO_Init(DISPLAY_CSX_PORT, &gpioConfig);

    gpioConfig.GPIO_Pin = DISPLAY_DCX_PIN;
    GPIO_Init(DISPLAY_DCX_PORT, &gpioConfig);

    DISPLAY_CS_HIGH();

    // SPI
    SPI_InitTypeDef  spiConfig;
    spiConfig.SPI_Direction = SPI_Direction_1Line_Tx;
    spiConfig.SPI_Mode = SPI_Mode_Master;
    spiConfig.SPI_DataSize = SPI_DataSize_8b;
    spiConfig.SPI_CPOL = SPI_CPOL_High;
    spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
    spiConfig.SPI_NSS = SPI_NSS_Soft;
    spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

    spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
    spiConfig.SPI_CRCPolynomial = 7;
    SPI_Init(DISPLAY_SPI, &spiConfig);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(DISPLAY_SPI, ENABLE);

//    /* Force reset */
//        ILI9341_RST_RESET;
//        vTaskDelay(20000);
//        ILI9341_RST_SET;

        /* Delay for RST response */
        vTaskDelay(20000);

        /* Software reset */
        SendCommand(ILI9341_RESET);
        vTaskDelay(50000);

        SendCommand(ILI9341_POWERA);
        SendData(0x39);
        SendData(0x2C);
        SendData(0x00);
        SendData(0x34);
        SendData(0x02);
        SendCommand(ILI9341_POWERB);
        SendData(0x00);
        SendData(0xC1);
        SendData(0x30);
        SendCommand(ILI9341_DTCA);
        SendData(0x85);
        SendData(0x00);
        SendData(0x78);
        SendCommand(ILI9341_DTCB);
        SendData(0x00);
        SendData(0x00);
        SendCommand(ILI9341_POWER_SEQ);
        SendData(0x64);
        SendData(0x03);
        SendData(0x12);
        SendData(0x81);
        SendCommand(ILI9341_PRC);
        SendData(0x20);
        SendCommand(ILI9341_POWER1);
        SendData(0x23);
        SendCommand(ILI9341_POWER2);
        SendData(0x10);
        SendCommand(ILI9341_VCOM1);
        SendData(0x3E);
        SendData(0x28);
        SendCommand(ILI9341_VCOM2);
        SendData(0x86);
        SendCommand(ILI9341_MAC);
        SendData(0x48);
        SendCommand(ILI9341_PIXEL_FORMAT);
        SendData(0x55);
        SendCommand(ILI9341_FRC);
        SendData(0x00);
        SendData(0x18);
        SendCommand(ILI9341_DFC);
        SendData(0x08);
        SendData(0x82);
        SendData(0x27);
        SendCommand(ILI9341_3GAMMA_EN);
        SendData(0x00);
        SendCommand(ILI9341_COLUMN_ADDR);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
        SendData(0xEF);
        SendCommand(ILI9341_PAGE_ADDR);
        SendData(0x00);
        SendData(0x00);
        SendData(0x01);
        SendData(0x3F);
        SendCommand(ILI9341_GAMMA);
        SendData(0x01);
        SendCommand(ILI9341_PGAMMA);
        SendData(0x0F);
        SendData(0x31);
        SendData(0x2B);
        SendData(0x0C);
        SendData(0x0E);
        SendData(0x08);
        SendData(0x4E);
        SendData(0xF1);
        SendData(0x37);
        SendData(0x07);
        SendData(0x10);
        SendData(0x03);
        SendData(0x0E);
        SendData(0x09);
        SendData(0x00);
        SendCommand(ILI9341_NGAMMA);
        SendData(0x00);
        SendData(0x0E);
        SendData(0x14);
        SendData(0x03);
        SendData(0x11);
        SendData(0x07);
        SendData(0x31);
        SendData(0xC1);
        SendData(0x48);
        SendData(0x08);
        SendData(0x0F);
        SendData(0x0C);
        SendData(0x31);
        SendData(0x36);
        SendData(0x0F);
        SendCommand(ILI9341_SLEEP_OUT);

        vTaskDelay(1000000);

        SendCommand(ILI9341_DISPLAY_ON);
    SendCommand(ILI9341_GRAM);
}

void DisplaySPI::SendCommand(uint8_t data) {
    DISPLAY_DCX_RESET();
    DISPLAY_CS_LOW();
    SPI_SendData(DISPLAY_SPI, data);
    DISPLAY_CS_HIGH();
}

void DisplaySPI::SendData(uint8_t data) {
    DISPLAY_DCX_SET();
    DISPLAY_CS_LOW();
    SPI_SendData(DISPLAY_SPI, data);
    DISPLAY_CS_HIGH();
}

void DisplaySPI::fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{

}
