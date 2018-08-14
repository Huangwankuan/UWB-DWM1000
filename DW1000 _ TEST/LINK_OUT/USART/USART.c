#include "USART.h"


///����DMA�ʼ�
/**
*@brief : ʹ��DMA���д��ڽ���
1.���ڳ�ʼ������
2.DMA��ʼ�����ã�ÿ��DMA��ʼ������ǰ��Ҫ���з���ʼ��Deinit��
3.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);�򿪴���DMA����


**/



UART_HandleTypeDef UART_Handler; //UART���
DMA_HandleTypeDef  UARTRxDMA_Handler;      //DMA���
#define Uart_size 8

u8 RX_BUF[Uart_size];
void UART_Init(void)
{
	UART_Handler.Instance=USART2;					    //USART1
	UART_Handler.Init.BaudRate=115200;				    //������
	UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	UART_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART_Handler);					    //HAL_UART_Init()��ʹ��UART1
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOC_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOF_CLK_ENABLE();			//ʹ��GPIOAʱ��
		
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

	
	}
	
}

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;      
	return ch;
}

