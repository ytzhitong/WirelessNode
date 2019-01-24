
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

//��ʼ�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������

//LED�������ȼ�
#define LED_TASK_PRIO		1
//�����ջ��С	
#define LED_STK_SIZE 		128  
//������
TaskHandle_t LEDTask_Handler;
//������
void start_task(void *pvParameters);

//TEST�������ȼ�
#define TEST_TASK_PRIO		1
//�����ջ��С	
#define TEST_STK_SIZE 		128  
//������
TaskHandle_t TESTTask_Handler;
//������
void TEST_task(void *pvParameters);

int main(void)
{	
	  HAL_Init();
    SystemClock_Config();	//��ʼ��ϵͳʱ��
    delay_init(); 		    //��ʼ����ʱ����  
	
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
				//��������
			Sys_Enter_Standby();
		}	

	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

    //����LED����
    xTaskCreate((TaskFunction_t )LED_task,     	
                (const char*    )"led_task",   	
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED_TASK_PRIO,	
                (TaskHandle_t*  )&LEDTask_Handler);  

    //����TEST����
//    xTaskCreate((TaskFunction_t )TEST_task,     	
//                (const char*    )"TEST_task",   	
//                (uint16_t       )TEST_STK_SIZE, 
//                (void*          )NULL,				
//                (UBaseType_t    )TEST_TASK_PRIO,	
//                (TaskHandle_t*  )&TESTTask_Handler); 								


    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//��������������
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

