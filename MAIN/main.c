
#include "sys.h"
#include "delay.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MY_GPIO.h"
#include "SHT2x.h"
#include "uart5.h"
#include "wkup.h"

u8 LoraTxTemp[8];

//author:ytzhitong

//开始任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数

//LED任务优先级
#define LED_TASK_PRIO		1
//任务堆栈大小	
#define LED_STK_SIZE 		128  
//任务句柄
TaskHandle_t LEDTask_Handler;
//任务函数
void start_task(void *pvParameters);

//TEST任务优先级
#define TEST_TASK_PRIO		1
//任务堆栈大小	
#define TEST_STK_SIZE 		128  
//任务句柄
TaskHandle_t TESTTask_Handler;
//任务函数
void TEST_task(void *pvParameters);

int main(void)
{	
	  HAL_Init();
    SystemClock_Config();	//初始化系统时钟
    delay_init(); 		    //初始化延时函数  
	
	  GPIO_Init();
  	SHT2x_Init();
	  uart5_init(115200);	
	  WKUP_Init();

    LED(1);	
	  delay_ms(20);
		SHT2x_Test();
		LORA_WAKE(0);
		delay_ms(5);		
		HAL_UART_Transmit(&UART5_Handler,g_sht2x_param.SHT_BT,8,1);
		LORA_WAKE(1);
	  LED(0);	
		
		if(Bit_SET == KEY_PIN_STATE()) 
		{		
				//进入休眠
			Sys_Enter_Standby();
		}	

	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

    //创建LED任务
    xTaskCreate((TaskFunction_t )LED_task,     	
                (const char*    )"led_task",   	
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED_TASK_PRIO,	
                (TaskHandle_t*  )&LEDTask_Handler);  

    //创建TEST任务
//    xTaskCreate((TaskFunction_t )TEST_task,     	
//                (const char*    )"TEST_task",   	
//                (uint16_t       )TEST_STK_SIZE, 
//                (void*          )NULL,				
//                (UBaseType_t    )TEST_TASK_PRIO,	
//                (TaskHandle_t*  )&TESTTask_Handler); 								


    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//测试任务任务函数
void TEST_task(void *pvParameters)
{
	while(1)
	{
		    LED_TogglePin;
		
		    SHT2x_Test();
		
		    //HAL_UART_Transmit_IT(&UART5_Handler,(uint8_t *)"hello world\n",12);
		    LORA_WAKE(0);
		    delay_ms(5);		
		    HAL_UART_Transmit(&UART5_Handler,g_sht2x_param.SHT_BT,8,1);
	    	LORA_WAKE(1);
		
//		    HAL_UART_Transmit_IT(&UART5_Handler,g_sht2x_param.HUMI_BT,4);
		
        vTaskDelay(60000);
	}	
}

