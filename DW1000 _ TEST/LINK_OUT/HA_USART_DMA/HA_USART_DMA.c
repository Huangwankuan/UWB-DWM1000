#include "HA_USART_DMA.h"
///����DMA�ʼ�
/**
*@brief : ʹ��DMA���д��ڽ���
1.���ڳ�ʼ������
2.DMA��ʼ�����ã�ÿ��DMA��ʼ������ǰ��Ҫ���з���ʼ��Deinit��
3.__HAL_LINKDMA(&USART_Handler,hdmarx,USARTRxDMA_Handler);
4.HAL_UART_Receive_DMA(&UART1_Handler,RX_BUF,Uart_size);�򿪴���DMA����
5.HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//����HAL���жϴ����ú���
6.void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TCIF3_7);//���DMA2_Steam7������ɱ�־
HAL_UART_DMAStop(&UART1_Handler);      //��������Ժ�رմ���DMA


����ô�����ȫ���������ж�  �����ǵ���DMA�Դ��ĺ���ӳ��
**/

UART_HandleTypeDef USART_Handler; //UART���
DMA_HandleTypeDef  USARTRxDMA_Handler;      //DMA���

#define Uart_size 18       //����DMA��������֡�ĳ���

u8 RX_BUF[Uart_size];

u8 UART_STA=0;
//[7]:1-˫����׼����
//[6];1-����У׼�ɹ�ָ��



/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//OLED_OUT_Num(1,4,j,10);
}*/
void DMA_IT_Re_Init(void)
{
			GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();           
		
		GPIO_Initure.Pin=GPIO_PIN_4; 
		GPIO_Initure.Mode=GPIO_MODE_IT_RISING ;
		GPIO_Initure.Pull=GPIO_PULLDOWN;        
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		HAL_NVIC_SetPriority(EXTI4_IRQn,0,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //ʹ���ж���2
}

void UART2_Init(void)
{
	USART_Handler.Instance=USART2;					   
	USART_Handler.Init.BaudRate=115200;				    //������
	USART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	USART_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	USART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	USART_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&USART_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	__HAL_UART_DISABLE(&USART_Handler);
	//Rx DMA����
	USARTRxDMA_Handler.Instance=DMA1_Stream5;                            //������ѡ��
	USARTRxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                                //ͨ��ѡ��
	USARTRxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;           
	USARTRxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	USARTRxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	USARTRxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	USARTRxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	USARTRxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //������ͨģʽ
	USARTRxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	USARTRxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	USARTRxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	USARTRxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	USARTRxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
//	__HAL_UART_ENABLE(&USART_Handler);
	HAL_DMA_DeInit(&USARTRxDMA_Handler);   
	HAL_DMA_Init(&USARTRxDMA_Handler);
	
//	HAL_DMA_RegisterCallback(&UART7RxDMA_Handler, HAL_DMA_XFER_CPLT_CB_ID, UART7_DMA_Accomplish_Callback);
//	HAL_DMA_RegisterCallback(&UART7RxDMA_Handler, HAL_DMA_XFER_HALFCPLT_CB_ID, UART7_DMA_HAlf);
//	

		__HAL_LINKDMA(&USART_Handler,hdmarx,USARTRxDMA_Handler);
	//	HAL_UART_Receive_DMA(&USART_Handler,RX_BUF,Uart_size);
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();					//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART1ʱ��
	  __HAL_RCC_DMA1_CLK_ENABLE();					//DMA2ʱ��ʹ��	
	
		GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			//PA2-U2_TX  PA3-U2_RX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	 
		
		HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);		
		HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,0,1);
		
			//	HAL_NVIC_EnableIRQ(USART2_IRQn);		
  	//	HAL_NVIC_SetPriority(USART2_IRQn,0,1);
	}	
}





void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&USARTRxDMA_Handler);	//����HAL���жϴ����ú���
}


//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}




void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//�����жϴ����ú���
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
				if(GPIO_Pin==GPIO_PIN_4) 
		{
	 __HAL_UART_ENABLE(&USART_Handler);
	//		HAL_UART_Receive_IT(&USART_Handler,RX_BUF,Uart_size);
		//tmp = (uint16_t)(USART_Handler.Instance->DR & (uint16_t)0x00FFU);
		HAL_UART_Receive_DMA(&USART_Handler,RX_BUF,Uart_size);
		}	
}



