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

#include <SPIDisplay.h>

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
#ifdef USE_FREERTOS
    const portTickType xDelayTime = 500 / portTICK_RATE_MS;
#endif
    while (1)
    {
        if(isSet)
            GPIO_ResetBits( GPIOG, GPIO_Pin_14 |GPIO_Pin_13 );
        else
            GPIO_SetBits( GPIOG, GPIO_Pin_14 |GPIO_Pin_13 );
        isSet = !isSet;
#ifdef USE_FREERTOS
        vTaskDelay(xDelayTime);
#else
        delay(500);
#endif
    }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

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
    SPIDisplay theDisplay;
    while(1)
    {
        GPIOG->BSRRL = GPIO_Pin_14 |GPIO_Pin_13;
        theDisplay.readID();
        GPIOG->BSRRH = GPIO_Pin_14 |GPIO_Pin_13;
        delay(100);
    }
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
//    toggleLed( nullptr);
}
#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
