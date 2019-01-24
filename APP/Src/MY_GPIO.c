
#include "sys.h"
#include "delay.h"
#include "MY_GPIO.h"
#include "FreeRTOS.h"
#include "task.h"

/**
  * @brief LED Initialization Function
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LORA_WAKE_PORT, LORA_WAKE_PIN, GPIO_PIN_SET);

  /*Configure GPIO pin :  */
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
	
  /*Configure GPIO pin :  */
  GPIO_InitStruct.Pin = LORA_WAKE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LORA_WAKE_PORT, &GPIO_InitStruct);
	

  GPIO_InitStruct.Pin =  KEY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(KEY_PORT, &GPIO_InitStruct);

}

void LED_task(void)
{
	while(1)
	{
		    LED_TogglePin;
        vTaskDelay(500);
	}
}

