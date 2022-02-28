


/*************************************************************************
 * Copyright (c) 2022,  WU JINCHANG
 * All rights reserved.
 * 
 * File name    :   MAIN.c
 * Brief        :   Pet_feeder code.
 *               Introduce the main function or content of this document briefly.
 * Revision     :   1.1
 * Author       :   WU JINCHANG
 * Date         :   2022.03.21
 * Update       :   Itroduce the difference from previous version.
 * Website      :		http://hinuohui.com/
*************************************************************************/


#include <reg51.h>
#include "lcd.h"
#include<intrins.h>
#include<stdio.h>
#include "DHT11.h"

//λ����
sbit _4IN = 	P2^0;				//�������IO
sbit BEEP = P2^1;
//��������

//----------------DHT11���ݱ�����-------------------------------
unsigned int rec_dat[4];
unsigned char rec_dat_lcd0[6];
unsigned char rec_dat_lcd1[6];
unsigned char rec_dat_lcd2[6];
unsigned char rec_dat_lcd3[6];

//----------------�¶���ֵ----------------

unsigned int Temp_High_threshold=30;
unsigned int Temp_LOW_threshold=20;
//----------------��ʱ��������-------------------------------


//��ʱʱ��Ͷι
int TIME_50ms_count;
int TIME_50ms_des=15*1.6;			//����Ϊ��λ


//��ʱʱ��Ͷι
int Motor_TIME_50ms_count;
int Motor_TIME_50ms_des=10*1.6;			//�������ʱ��


unsigned int Time_Mod=0;		// 0:��ʱģʽ  1:��ʳģʽ			
unsigned int Time_Mod_B=0;		// ģʽ�����־λ  0����¼�  1δ����¼�



//��������
void key_scanf(void);
void gx_teep(void);
void DHT11_delay_ms(unsigned int z);
void DHT11_start();
unsigned char DHT11_rec_byte();
void DHT11_receive();

//��ʱ��0��ʼ��
void Time0_inint(void);




void delay(unsigned int z)//���뼶��ʱ
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}


//��ʱ��0�ж�
void timer0() interrupt 1
{
	
		
	if(Time_Mod==0)		//��ʱģʽ
	{
	//��ʱ���������65���˳���ʹ��50ms�ж�
		TIME_50ms_count++;
		if(TIME_50ms_count==20*TIME_50ms_des)		//20��* 50ms =1��
		{
			Time_Mod=1;						//�����ʳģʽ
			TIME_50ms_count=0;		//��������
				
		}
		
		
	}
	if(Time_Mod==1)									//��ʳģʽ
	{
		//��ʱ���������65���˳���ʹ��50ms�ж�
		Motor_TIME_50ms_count++;
		
		if(Motor_TIME_50ms_count>20*Motor_TIME_50ms_des)	//��ʱ���������65���˳���ʹ��50ms�ж�
		{
					
			Time_Mod=0;									//���붨ʱģʽ
			Motor_TIME_50ms_count=0		//��������
		}
		
		
	}	

				
}


void main()
{
	
	_4IN = 0;			//�ϵ籣֤����ת,ǿ������
	P1=0xf0;
	InitLcd1602();						//��ʼ��LCD
	LcdShowStr(0,1,"Temp:");	//���»���
	gx_teep();			//����ʱ��
	Time0_inint();	//��ʱ��0���ó�ʼ��
	_4IN = 0;				//�ϵ籣֤����ת,ǿ������
	while(1)
	{
	//�ж�ģʽ
		if(Time_Mod==0)		//��ʱģʽ
		{
			_4IN=0;		//�رյ��

		}
		if(Time_Mod==1)			//��ʳģʽ
		{
			
			_4IN=1;		//�������
		}

		//��������
		gx_teep();

		//�¶ȱ������
		if((rec_dat[2]>=Temp_High_threshold) | (Temp_LOW_threshold>rec_dat[2]))	//�¶���ֵ�ж�
		{
			
			BEEP=0;		//�򿪷���������
			
			
		}else{
			BEEP=1;		//�رշ���������
			
			
		}
		
		//while ѭ��̫�죬������϶�ʱ�����˴�500���Ը���ʵ���������	������̫��
		DHT11_delay_ms(500);

			
	}
}


//��ʱ��0����
void Time0_inint(){
	
    //����ʱ��
   TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ʼֵ
	TH0 = 0x4C;		//���ö�ʱ��ʼֵ
    
    ET0=1;	//�򿪶�ʱ���ж�
    EA=1;	//�����ж�
    TR0=1;	//������ʱ��
}


//�����¶�,��ʾ��LCD
void gx_teep()
{

		TR0=0;	//�رն�ʱ��	
		DHT11_delay_ms(20);
		DHT11_receive();

	
		sprintf(rec_dat_lcd2,"%d",rec_dat[2]);
		sprintf(rec_dat_lcd3,"%d",rec_dat[3]);
		DHT11_delay_ms(13);
		

		
		//�¶�
		LcdShowStr(6,1,rec_dat_lcd2);
		LcdShowStr(8,1,".");
		LcdShowStr(9,1,rec_dat_lcd3);
		LcdShowStr(10,1," C");
		TR0=1;//�򿪶�ʱ��	

}