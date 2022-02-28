
#include<reg51.h>

#ifndef __DHT11_H__
#define __HDT11_H__

//DHT11引脚定义
sbit Temp_data=P3^7;


extern unsigned int rec_dat[4];
//unsigned char data_byte;
//unsigned char rec_dat_lcd=[4];
extern unsigned char rec_dat_lcd0[6];
extern unsigned char rec_dat_lcd1[6];
extern unsigned char rec_dat_lcd2[6];
extern unsigned char rec_dat_lcd3[6];


//定义
void DHT11_delay_us(unsigned char n);
void DHT11_delay_ms(unsigned int z);
void DHT11_start(void);
unsigned char DHT11_rec_byte(void);
void DHT11_receive(void);

#endif