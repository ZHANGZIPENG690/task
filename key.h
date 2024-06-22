#ifndef _KEY_H_
#define _KEY_H_
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "sys.h"
#include "stdio.h"

//#define KEY1_EVENT (0x01 << 0)//设置事件掩码的位 0
//#define KEY2_EVENT (0x01 << 1)//设置事件掩码的位 1

typedef struct
{
	uint8_t jiemian1;//界面区分标志位
	uint8_t jiemian2;//界面2的标志位
}BIAOZHIWEI;

extern BIAOZHIWEI bz;

#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//读取按键0
#define KEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 
#define KEY_WK_UP 2
#define KEY3_PC8 3
#define KEY4_PC9 4 

void KEY_Config(void);//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数
void KEY_Handle(void);

#endif