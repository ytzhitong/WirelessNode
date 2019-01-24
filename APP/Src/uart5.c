#include "uart5.h"

//串口5中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;     //接收状态标记

UART_HandleTypeDef UART5_Handler; //UART5句柄


/**
 * @brief	初始化串口1函数
 *
 * @param	bound	串口波特率
 *
 * @return  void
 */
void uart5_init(u32 bound)
{
    //UART 初始化设置
	
		UART5_Handler.Instance = UART5;
		UART5_Handler.Init.BaudRate = 115200;
		UART5_Handler.Init.WordLength = UART_WORDLENGTH_8B;
		UART5_Handler.Init.StopBits = UART_STOPBITS_1;
		UART5_Handler.Init.Parity = UART_PARITY_NONE;
		UART5_Handler.Init.Mode = UART_MODE_TX_RX;
		UART5_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
		HAL_UART_Init(&UART5_Handler);

    __HAL_UART_DISABLE_IT(&UART5_Handler, UART_IT_RXNE); //开启接收中断
    HAL_NVIC_EnableIRQ(UART5_IRQn);					//使能USART1中断通道
    HAL_NVIC_SetPriority(UART5_IRQn, 3, 3);				//抢占优先级3，子优先级3
}

/**
 * @brief	串口1中断服务程序
 *
 * @remark	下面代码我们直接把中断控制逻辑写在中断服务函数内部
 * 			说明：采用HAL库处理逻辑，效率不高。
 *
 * @param   void
 *
 * @return  void
 */
void UART5_IRQHandler(void)
{
    u8 Res;

    if((__HAL_UART_GET_FLAG(&UART5_Handler, UART_FLAG_RXNE) != RESET)) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        HAL_UART_Receive(&UART5_Handler, &Res, 1, 1000);

        if((USART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if(USART_RX_STA & 0x4000) //接收到了0x0d
            {
                if(Res != 0x0a)USART_RX_STA = 0; //接收错误,重新开始

                else USART_RX_STA |= 0x8000;	//接收完成了
            }
            else //还没收到0X0D
            {
                if(Res == 0x0d)USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
                    USART_RX_STA++;

                    if(USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
    HAL_UART_IRQHandler(&UART5_Handler);
}








