


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

//位定义
sbit _4IN = 	P2^0;				//电机控制IO
sbit BEEP = P2^1;
//变量声明

//----------------DHT11数据变量区-------------------------------
unsigned int rec_dat[4];
unsigned char rec_dat_lcd0[6];
unsigned char rec_dat_lcd1[6];
unsigned char rec_dat_lcd2[6];
unsigned char rec_dat_lcd3[6];

//----------------温度阈值----------------

unsigned int Temp_High_threshold=30;
unsigned int Temp_LOW_threshold=20;
//----------------定时器变量区-------------------------------


//定时时间投喂
int TIME_50ms_count;
int TIME_50ms_des=15*1.6;			//以秒为单位


//定时时间投喂
int Motor_TIME_50ms_count;
int Motor_TIME_50ms_des=10*1.6;			//电机运行时间


unsigned int Time_Mod=0;		// 0:定时模式  1:出食模式			
unsigned int Time_Mod_B=0;		// 模式缓冲标志位  0完成事件  1未完成事件



//函数声明
void key_scanf(void);
void gx_teep(void);
void DHT11_delay_ms(unsigned int z);
void DHT11_start();
unsigned char DHT11_rec_byte();
void DHT11_receive();

//定时器0初始化
void Time0_inint(void);




void delay(unsigned int z)//毫秒级延时
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}


//定时器0中断
void timer0() interrupt 1
{
	
		
	if(Time_Mod==0)		//定时模式
	{
	//定时器最大周期65，此程序使用50ms中断
		TIME_50ms_count++;
		if(TIME_50ms_count==20*TIME_50ms_des)		//20次* 50ms =1秒
		{
			Time_Mod=1;						//进入出食模式
			TIME_50ms_count=0;		//计数清零
				
		}
		
		
	}
	if(Time_Mod==1)									//出食模式
	{
		//定时器最大周期65，此程序使用50ms中断
		Motor_TIME_50ms_count++;
		
		if(Motor_TIME_50ms_count>20*Motor_TIME_50ms_des)	//定时器最大周期65，此程序使用50ms中断
		{
					
			Time_Mod=0;									//进入定时模式
			Motor_TIME_50ms_count=0		//计数清零
		}
		
		
	}	

				
}


void main()
{
	
	_4IN = 0;			//上电保证不会转,强制拉低
	P1=0xf0;
	InitLcd1602();						//初始化LCD
	LcdShowStr(0,1,"Temp:");	//更新画面
	gx_teep();			//更新时间
	Time0_inint();	//定时器0配置初始化
	_4IN = 0;				//上电保证不会转,强制拉低
	while(1)
	{
	//判断模式
		if(Time_Mod==0)		//定时模式
		{
			_4IN=0;		//关闭电机

		}
		if(Time_Mod==1)			//出食模式
		{
			
			_4IN=1;		//启动电机
		}

		//更新天气
		gx_teep();

		//温度报警检测
		if((rec_dat[2]>=Temp_High_threshold) | (Temp_LOW_threshold>rec_dat[2]))	//温度阈值判断
		{
			
			BEEP=0;		//打开蜂鸣器报警
			
			
		}else{
			BEEP=1;		//关闭蜂鸣器报警
			
			
		}
		
		//while 循环太快，经常打断定时器，此处500可以根据实际情况调节	，不可太低
		DHT11_delay_ms(500);

			
	}
}


//定时器0配置
void Time0_inint(){
	
    //配置时间
   TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初始值
	TH0 = 0x4C;		//设置定时初始值
    
    ET0=1;	//打开定时器中断
    EA=1;	//打开总中断
    TR0=1;	//启动定时器
}


//更新温度,显示到LCD
void gx_teep()
{

		TR0=0;	//关闭定时器	
		DHT11_delay_ms(20);
		DHT11_receive();

	
		sprintf(rec_dat_lcd2,"%d",rec_dat[2]);
		sprintf(rec_dat_lcd3,"%d",rec_dat[3]);
		DHT11_delay_ms(13);
		

		
		//温度
		LcdShowStr(6,1,rec_dat_lcd2);
		LcdShowStr(8,1,".");
		LcdShowStr(9,1,rec_dat_lcd3);
		LcdShowStr(10,1," C");
		TR0=1;//打开定时器	

}