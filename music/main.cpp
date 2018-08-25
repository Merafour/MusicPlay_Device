/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : main.cpp
* Author             : Merafour
* Last Modified Date : 08/11/2018
* Description        : main.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

#include "usbd_cdc_if.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include <string.h>
#include "music.h"
#include "RTTTL.h"
#include "online.h"



/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
	
#ifdef __cplusplus
 extern "C" {
#endif

extern void delay_ms(uint16_t ms);
extern uint32_t sys_tick(void);
//int main(void);

#ifdef __cplusplus
}
#endif
void delay_ms(uint16_t ms)
{
	//HAL_Delay(ms);
    osDelay(ms);
}
uint32_t sys_tick(void)
{
  return HAL_GetTick();
}

void Music::delay_ms(uint16_t ms)
{
	//HAL_Delay(ms);
    osDelay(ms);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
extern void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

beep_speaker _speaker;

Music _music(_speaker);
RTTTL _rtttl(_speaker);
//#define SNUM  LED.get_lenght()
/* USER CODE END 0 */
void User_PWM_SetPulseValue(uint16_t pulseValue)
{
  TIM_OC_InitTypeDef sConfigOC;
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = pulseValue;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  
}
extern void Pulse_frequency_Value(uint16_t frequency, uint16_t pulseValue);
extern void Pulse_note_Value(uint8_t note);

/** @defgroup Vector_Table_Base 
  * @{
  */

#define NVIC_VectTab_RAM             ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH           ((uint32_t)0x08000000)
#define IS_NVIC_VECTTAB(VECTTAB) (((VECTTAB) == NVIC_VectTab_RAM) || \
                                  ((VECTTAB) == NVIC_VectTab_FLASH))
/**
  * @brief  Sets the vector table location and Offset.
  * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
  *   This parameter can be one of the following values:
  *     @arg NVIC_VectTab_RAM: Vector Table in internal SRAM.
  *     @arg NVIC_VectTab_FLASH: Vector Table in internal FLASH.
  * @param  Offset: Vector Table base offset field. This value must be a multiple of 0x200.
  * @retval None
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* Check the parameters */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}
void USB_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_DM_GPIO_Port, USB_DM_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(USB_DP_GPIO_Port, USB_DP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = USB_DM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USB_DM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USB_DP_GPIO_Port, &GPIO_InitStruct);

}

extern osThreadId defaultTaskHandle;
extern osThreadId MusicTaskHandle;
osThreadId OnlineTxTaskHandle;
void StartOnlineTxTask(void const * argument)
{
    static uint8_t send_buf[64];
    uint8_t len=0;
    int ch=-1;
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
        osDelay(2);
      for(len=0; len<64; len++)
      {
          ch = tx_buf_get();
          if(ch<0) break;
          send_buf[len] = ch;
      }
      if(len>0)
      {
          CDC_Transmit_FS(send_buf, len);
      }
  }
  /* USER CODE END StartDefaultTask */
}
static uint8_t order=0;
/* StartDefaultTask function */
void _StartDefaultTask(void const * argument)
{
    uint8_t cmd=0;
    uint16_t i=0;
    uint16_t pos=0;
    uint16_t len=0;
    uint8_t ch=0;
    const uint8_t* music;
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    cmd = online_update();
    // play
      switch(cmd)
      {
          case PROTO_MUSIC:
              memset(_music._rtttl_buf, 0, sizeof(_music._rtttl_buf));
              memset(_music._melody_buf, 0, sizeof(_music._melody_buf));
              memset(_music._octave_buf, 0, sizeof(_music._octave_buf));
              memset(_music._durations_buf, 0, sizeof(_music._durations_buf));
              music = get_music();
              len = strlen((char*)music);
              pos=0;
              for(i=0; i<len; i++)
              {
                  ch = music[i];
                  if('\n'==ch) break;
                  _music._rtttl_buf[pos++] = ch;
              }
              pos=0;
              for(i++; i<len; i++)
              {
                  ch = music[i];
                  if('\n'==ch) break;
                  _music._melody_buf[pos++] = ch;
              }
              pos=0;
              for(i++; i<len; i++)
              {
                  ch = music[i];
                  if('\n'==ch) break;
                  _music._octave_buf[pos++] = ch;
              }
              pos=0;
              for(i++; i<len; i++)
              {
                  ch = music[i];
                  if(('\n'==ch) || ('\0'==ch)) break;
                  _music._durations_buf[pos++] = ch;
              }
              break;
          case PROTO_PLAY:   // play
              break;
          case PROTO_PAUSE:  // pause
              _rtttl.pause();
              break;
          case PROTO_NEXT:   // next play
              break;
          case PROTO_LAST:   // last play
              break;
          default:
              break;
      }
      order = cmd;
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* StartMusicTask function */
void _StartMusicTask(void const * argument)
{
  /* USER CODE BEGIN StartMusicTask */
  /* Infinite loop */
#if 0
	_rtttl.Init();
	_rtttl.test();
#endif

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {
  /* USER CODE END WHILE */
      if(PROTO_PLAY==order)
      {
          order = 0;
          _rtttl.play();
      }
  /* USER CODE BEGIN 3 */
//	_music.Init();
//	_music.test();
    osDelay(10);
#if 0
      osDelay(3000);
      buf_put(0x12);
      buf_put(0x50);
#endif
  }
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMusicTask */
}
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
    USB_StopDevice(USB);
    USB_GPIO_Init();
    //HAL_Delay(500);
	HAL_Delay(500);
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();

    //MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	//HAL_Delay(500);
  /* Call init function for freertos objects (in freertos.c) */
  //MX_FREERTOS_Init();
  
    /* Create the thread(s) */
  /* definition and creation of defaultTask */
  // 接收串口数据并处理上位机发过来的指令
  osThreadDef(defaultTask, _StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  // 发送数据到串口线程
  osThreadDef(OnlineTxTask, StartOnlineTxTask, osPriorityNormal, 0, 128);
  OnlineTxTaskHandle = osThreadCreate(osThread(OnlineTxTask), NULL);

  /* definition and creation of MusicTask */
  // 音乐播放线程
  osThreadDef(MusicTask, _StartMusicTask, osPriorityRealtime, 0, 128);
  MusicTaskHandle = osThreadCreate(osThread(MusicTask), NULL);

  /* Start scheduler */
  osKernelStart();
  while(1) osDelay(1);
#if 1
	_rtttl.Init();
	_rtttl.test();
#endif

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {
  /* USER CODE END WHILE */
	
  /* USER CODE BEGIN 3 */
	_music.Init();
	_music.test();
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
