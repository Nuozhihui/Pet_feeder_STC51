
#include <reg51.h>
#include "lcd.h"
#include<intrins.h>
#include<stdio.h>
#include "DHT11.h"

sbit LED1 = P1^3;
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


//��ʳ��
int PWM_COT=9600*1;
	
int PWM_COT_C=0;
sbit _4IN = 	P2^0;//
sbit key_s2 = P3^0;
sbit key_s3 = P3^1;

void delay(unsigned int z)//���뼶��ʱ
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}


//��ʱ��0�ж�
void timer0() interrupt 1
{
	
	
	if(Time_Mod==0)
	{

		TIME_50ms_count++;
		if(TIME_50ms_count==20*TIME_50ms_des)
		{
			Time_Mod=1;
			TIME_50ms_count=0;
				
		}
		
		
	}
	if(Time_Mod==1)
	{
		
		Motor_TIME_50ms_count++;
		if(Motor_TIME_50ms_count>20*Motor_TIME_50ms_des)
		{
					
			Time_Mod=0;
			Motor_TIME_50ms_count=0;
		}
		
		
	}	
//	
////	if(TIME_50ms_count % 20==0 || Motor_TIME_50ms_count % 20==0 )
////	{
////		
////		
////	}
				
}


void main()
{
	
	_4IN = 0;
	P1=0xf0;
	InitLcd1602();
//	LcdShowStr(0,0,"Humi:");
	LcdShowStr(0,1,"Temp:");
	gx_teep();
	//Timer1Init();			//ʱ��
	Time0_inint();
	_4IN = 0;
	while(1)
	{
	
		if(Time_Mod==0)
		{
			_4IN=0;		//�������

		}
		if(Time_Mod==1)			//��ʳģʽ
		{
			
			_4IN=1;		//�������
		}

gx_teep();

			if((rec_dat[2]>=Temp_High_threshold) | (Temp_LOW_threshold>rec_dat[2]))	//�¶���ֵ�ж�
			{
				
				BEEP=0;		//�򿪷���������
				
				
			}else{
				BEEP=1;		//�رշ���������
				
				
			}
		
		DHT11_delay_ms(500);

			
	}
}


void Time0_inint(){
	
    //����ʱ��
   TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ʼֵ
	TH0 = 0x4C;		//���ö�ʱ��ʼֵ
    
    ET0=1;	//�򿪶�ʱ���ж�
    EA=1;	//�����ж�
    TR0=1;	//������ʱ��
}

////����ɨ��
//void key_scanf()
//{
//		if(key_s2 == 0)	 //S2����LED�䰵
//		{
//			delay(5);
//			if(key_s2 == 0)
//			{
//				if(pwm_motor_val < 250)
//				{
//					pwm_motor_val++;
//				}
//			}
//		}
//		if(key_s3 == 0)	//S3����LED����
//		{
//			delay(5);
//			if(key_s3 == 0)
//			{
//				if(pwm_motor_val > 0)
//				{
//					pwm_motor_val--;
//				}
//			}
//		}	
//	
//	
//	
//}
//�����¶�
void gx_teep()
{

		TR0=0;
		DHT11_delay_ms(20);
		DHT11_receive();

		
//		sprintf(rec_dat_lcd0,"%d",rec_dat[0]);
//		sprintf(rec_dat_lcd1,"%d",rec_dat[1]);
		sprintf(rec_dat_lcd2,"%d",rec_dat[2]);
		sprintf(rec_dat_lcd3,"%d",rec_dat[3]);
		DHT11_delay_ms(13);
		
//		//ʪ��
//		LcdShowStr(6,0,rec_dat_lcd0);
//		LcdShowStr(8,0,".");
//		LcdShowStr(9,0,rec_dat_lcd1);
//		LcdShowStr(10,0," %");
		
		//�¶�
		LcdShowStr(6,1,rec_dat_lcd2);
		LcdShowStr(8,1,".");
		LcdShowStr(9,1,rec_dat_lcd3);
		LcdShowStr(10,1," C");
		TR0=1;

}