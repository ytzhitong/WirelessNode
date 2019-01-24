#include "uart5.h"

//����5�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0;     //����״̬���

UART_HandleTypeDef UART5_Handler; //UART5���


/**
 * @brief	��ʼ������1����
 *
 * @param	bound	���ڲ�����
 *
 * @return  void
 */
void uart5_init(u32 bound)
{
    //UART ��ʼ������
	
		UART5_Handler.Instance = UART5;
		UART5_Handler.Init.BaudRate = 115200;
		UART5_Handler.Init.WordLength = UART_WORDLENGTH_8B;
		UART5_Handler.Init.StopBits = UART_STOPBITS_1;
		UART5_Handler.Init.Parity = UART_PARITY_NONE;
		UART5_Handler.Init.Mode = UART_MODE_TX_RX;
		UART5_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
		HAL_UART_Init(&UART5_Handler);

    __HAL_UART_DISABLE_IT(&UART5_Handler, UART_IT_RXNE); //���������ж�
    HAL_NVIC_EnableIRQ(UART5_IRQn);					//ʹ��USART1�ж�ͨ��
    HAL_NVIC_SetPriority(UART5_IRQn, 3, 3);				//��ռ���ȼ�3�������ȼ�3
}

/**
 * @brief	����1�жϷ������
 *
 * @remark	�����������ֱ�Ӱ��жϿ����߼�д���жϷ������ڲ�
 * 			˵��������HAL�⴦���߼���Ч�ʲ��ߡ�
 *
 * @param   void
 *
 * @return  void
 */
void UART5_IRQHandler(void)
{
    u8 Res;

    if((__HAL_UART_GET_FLAG(&UART5_Handler, UART_FLAG_RXNE) != RESET)) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        HAL_UART_Receive(&UART5_Handler, &Res, 1, 1000);

        if((USART_RX_STA & 0x8000) == 0) //����δ���
        {
            if(USART_RX_STA & 0x4000) //���յ���0x0d
            {
                if(Res != 0x0a)USART_RX_STA = 0; //���մ���,���¿�ʼ

                else USART_RX_STA |= 0x8000;	//���������
            }
            else //��û�յ�0X0D
            {
                if(Res == 0x0d)USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
                    USART_RX_STA++;

                    if(USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
    HAL_UART_IRQHandler(&UART5_Handler);
}








