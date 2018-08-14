#include "CAN.h"

CAN_HandleTypeDef   CAN1_Handler;   //CAN1���
CanTxMsgTypeDef     TxMessage;      //������Ϣ
CanRxMsgTypeDef     RxMessage;      //������Ϣ

////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+tbs2+1)*brp); ����tbs1��tbs2����ֻ�ù�ע��ʶ���ϱ�־����ţ�����CAN_BS2_1TQ�����Ǿ���Ϊtbs2=1�����㼴�ɡ�
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ45M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_8tq,6,CAN_MODE_LOOPBACK);
//������Ϊ:45M/((6+8+1)*6)=500Kbps

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    u16 Filter_ID1,Filter_MASK_1;
		u16 Filter_ID2,Filter_MASK_2;
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&TxMessage;     //������Ϣ
    CAN1_Handler.pRxMsg=&RxMessage;     //������Ϣ
    CAN1_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN1_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN1_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN1_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
	
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //��ʼ��
    
		Filter_ID1 = 0x0000;
		Filter_MASK_1 = 0x0000;	
		Filter_ID2 = 0x0000;
    Filter_MASK_2	=	0x0000;
    CAN1_FilerConf.FilterIdHigh=Filter_ID2;     //32λID
    CAN1_FilerConf.FilterIdLow=Filter_ID1;
    CAN1_FilerConf.FilterMaskIdHigh=Filter_MASK_2; //32λMASK
    CAN1_FilerConf.FilterMaskIdLow=Filter_MASK_1;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN1_FilerConf.FilterNumber=0;          //������0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_16BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN1_FilerConf.BankNumber=14;//CAN2���������14��ʼ
	
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
	
    return 0;
}

//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();			    //����GPIOAʱ��
	  __HAL_RCC_GPIOC_CLK_ENABLE();			    //����GPIOAʱ��
		
		
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;   //
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //����ΪCAN1
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);         //��ʼ��
		
		GPIO_Initure.Pin=GPIO_PIN_2;   //
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLDOWN;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //��ʼ��
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);

	  CAN1_Handler.State=HAL_CAN_STATE_READY;//�������CAN1״̬ ���𽫲���򿪽����ж�
    while(HAL_CAN_Receive_IT(&CAN1_Handler,CAN_FIFO0)!=HAL_OK);//ʹ�øú��� ����ʹ��������ص�CAN�ж�
		//__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//Ҳ��ֱ�Ӵ�FIFO�����ж�
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,0,0); 
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);       
}

void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);//
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    if(hcan->Instance==CAN1)
		{
		  
			/*int i=0;
			printf("�������\n");
			
			printf("ID:%d\r\n",CAN1_Handler.pRxMsg->StdId);
			printf("IDE:%d\r\n",CAN1_Handler.pRxMsg->IDE);
			printf("RTR:%d\r\n",CAN1_Handler.pRxMsg->RTR);
			printf("SIZE:%d\r\n",CAN1_Handler.pRxMsg->DLC);
			for(i=0;i<8;i++)
			printf("RXBUF[%d]:%d\r\n",i,CAN1_Handler.pRxMsg->Data[i]);*/
			
			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
		}
}	
//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 size)
{	
    u16 i=0;
    CAN1_Handler.pTxMsg->StdId=0X12;        //��׼��ʶ��
    CAN1_Handler.pTxMsg->ExtId=0;        //��չ��ʶ��(29λ)
    CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //ʹ�ñ�׼֡
    CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //����֡
    CAN1_Handler.pTxMsg->DLC=size;                
    for(i=0;i<size;i++)
    CAN1_Handler.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //����
    return 0;		
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//�������ݣ���ʱʱ������Ϊ0	
    for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
    buf[i]=CAN1_Handler.pRxMsg->Data[i];
	return CAN1_Handler.pRxMsg->DLC;	
}

