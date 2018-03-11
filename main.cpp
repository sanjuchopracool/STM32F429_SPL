/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "system.h"

#include "SPI5Driver.h"

//#include "DisplaySPI.h"

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

void toggleLed(void * data)
{
    UNUSED(data);
    /* Infinite loop */
    int8_t isSet = 0;
    const portTickType xDelayTime = 500 / portTICK_RATE_MS;
    while (1)
    {
        if(isSet)
            GPIO_ResetBits( GPIOG, GPIO_Pin_14 |GPIO_Pin_13 );
        else
            GPIO_SetBits( GPIOG, GPIO_Pin_14 |GPIO_Pin_13 );
        isSet = !isSet;
        vTaskDelay(xDelayTime);
    }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

#define GYRO_LOW    GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define GYRO_High    GPIO_SetBits(GPIOC, GPIO_Pin_1)

#define READWRITE_CMD              ((uint8_t)0x80)
#define L3GD20_WHO_AM_I_ADDR          0x0F  /* device identification register */

int main(void)
{
    systemInit();
    // for error blink
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE );

    GPIO_InitTypeDef        GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_14 |GPIO_Pin_13);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure);


    // PC1 (CS for gyro)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GYRO_High;

    SPI5Driver theDriver;
    volatile uint16_t deviceId = 0;

    uint8_t inReg = (READWRITE_CMD | L3GD20_WHO_AM_I_ADDR);
    GYRO_LOW;
    deviceId = theDriver.WriteRead(inReg);
    deviceId = theDriver.WriteRead(0x00);
    GYRO_High;


    if( deviceId == 0xD4 )
        GPIO_SetBits( GPIOG, GPIO_Pin_14 |GPIO_Pin_13 );

//    xTaskCreate( toggleLed, "VTask", 1024, NULL,1,NULL);
//    vTaskStartScheduler();
    while(1);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    UNUSED(file);
    UNUSED(line);
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
