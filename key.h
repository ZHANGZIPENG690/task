#ifndef _KEY_H_
#define _KEY_H_
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "sys.h"
#include "stdio.h"

//#define KEY1_EVENT (0x01 << 0)//�����¼������λ 0
//#define KEY2_EVENT (0x01 << 1)//�����¼������λ 1

typedef struct
{
	uint8_t jiemian1;//�������ֱ�־λ
	uint8_t jiemian2;//����2�ı�־λ
}BIAOZHIWEI;

extern BIAOZHIWEI bz;

#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//��ȡ����0
#define KEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
 
#define KEY_WK_UP 2
#define KEY3_PC8 3
#define KEY4_PC9 4 

void KEY_Config(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��
void KEY_Handle(void);

#endif