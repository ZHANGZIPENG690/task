#include "key.h"
#include "shishicaozuo.h"

BIAOZHIWEI bz={0,0};

void KEY_Config(void)//开时钟
{
	//标准库版本
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC8
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC9
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
}

u8 KEY_Scan(void)
{	 
	uint8_t key_Value =0;//按键返回值
	//按键 WK-UP 控制
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//key1按键按下时高电平,所以等于0按键没按下，等于1按下
	{
			delay_ms(10);//消抖
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//二次确认是否按
			{
					//按键真的按下了
					while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!= Bit_RESET)//松手检测
					{}
					//按键松手
					key_Value = KEY_WK_UP;
			}
	}
	//按键3控制
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//key2按键按下时低电平,所以等于1按键没按下，等于0按下
	{
		delay_ms(10);//消抖
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//二次确认是否按
		{
			//按键真的按下了
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)== Bit_RESET)//松手检测
			{}
			//按键松手
				key_Value = KEY3_PC8;
		}
	}
	//按键3控制
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//key2按键按下时低电平,所以等于1按键没按下，等于0按下
	{
		delay_ms(10);//消抖
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//二次确认是否按
		{
			//按键真的按下了
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)== Bit_RESET)//松手检测
			{}
			//按键松手
				key_Value = KEY4_PC9;
		}
	}
	return key_Value;
}



void KEY_Handle(void)
{
	u8 t=0;
	t=KEY_Scan();		//得到键值
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
