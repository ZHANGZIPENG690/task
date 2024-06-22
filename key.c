#include "key.h"
#include "shishicaozuo.h"

BIAOZHIWEI bz={0,0};

void KEY_Config(void)//��ʱ��
{
	//��׼��汾
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC8
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC9
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
}

u8 KEY_Scan(void)
{	 
	uint8_t key_Value =0;//��������ֵ
	//���� WK-UP ����
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//key1��������ʱ�ߵ�ƽ,���Ե���0����û���£�����1����
	{
			delay_ms(10);//����
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//����ȷ���Ƿ�
			{
					//������İ�����
					while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//���ּ��
					{}
					//��������
					key_Value = KEY_WK_UP;
			}
	}
	//����3����
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//key2��������ʱ�͵�ƽ,���Ե���1����û���£�����0����
	{
		delay_ms(10);//����
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//����ȷ���Ƿ�
		{
			//������İ�����
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//���ּ��
			{}
			//��������
				key_Value = KEY3_PC8;
		}
	}
	//����3����
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//key2��������ʱ�͵�ƽ,���Ե���1����û���£�����0����
	{
		delay_ms(10);//����
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//����ȷ���Ƿ�
		{
			//������İ�����
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//���ּ��
			{}
			//��������
				key_Value = KEY4_PC9;
		}
	}
	return key_Value;
}



void KEY_Handle(void)
{
	u8 t=0;
	t=KEY_Scan();		//�õ���ֵ
	switch(t)
	{				 
		case KEY_WK_UP:
			beep_ON();
		printf("111\r\n");
		relays_ON();
			
			break;
		case KEY3_PC8:
			printf("222\r\n");
			beep_OFF();
			relays_OFF();
			break;	
		case KEY4_PC9:	
			beep_TOGGLE();
			break;
		default:	
			break;
	} 
}
