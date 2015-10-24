#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit keyfive=P3^0;
sbit keysix=P3^1;
sbit keyseven=P3^2;
sbit keyeight=P3^3;
bit keyfive_flag=0;
bit keysix_flag=0;
bit keyseven_flag=0;
bit keyeight_flag=0;
uchar a,b;
uchar alarm_panel[]="ALARM  PANEL TH: TL:";

/********调用外部函数及变量声明部分*************/
void ds18b20_writebyte(uchar dat);
uchar ds18b20_readbyte();
void ds18b20_initial();
void ds18b20_convert();
uint ds18b20_deal();
void lcd_writecommand(uchar command);
void lcd_writedata(uchar dat);
void lcd_initial();
uchar decimal_character(uchar decimal);
/********调用外部函数及变量声明部分*************/

/******************************
*  函数功能：软件延时（约1ms）
*  入口参数：uint xms
*  出口参数：
*******************************/
static void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
/***************按键检测程序****************/
void keyfive_scan()
{
	if(keyfive==0)
	{
		delayms(10);
		if(keyfive==0)
		{	
			while(keyfive==0);
			keyfive_flag=1;
		}
	}
}
void keysix_scan()
{
	if(keysix==0)
	{
		delayms(10);
		if(keysix==0)
		{	
			while(keysix==0);
			keysix_flag=1;
		}
	}
}
void keyseven_scan()
{
	if(keyseven==0)
	{
		delayms(10);
		if(keyseven==0)
		{	
			while(keyseven==0);
			keyseven_flag=1;
		}
	}
}
void keyeight_scan()
{
	if(keyeight==0)
	{
		delayms(10);
		if(keyeight==0)
		{	
			while(keyeight==0);
			keyeight_flag=1;
		}
	}
}
/*******************************************************
*  函数功能：向scratchpad TH TL CONFIGURATION中写入数据
*  入口参数：
*  出口参数：
*******************************************************/
void ds18b20_writealarm(uchar th,uchar tl)
{
	ds18b20_initial();
	ds18b20_writebyte(0xcc); //Skip ROM command
	ds18b20_writebyte(0x4e); //Write scratchpad
	ds18b20_writebyte(th); //TH 25
	ds18b20_writebyte(tl); //TL 0
	ds18b20_writebyte(0x7f); //configuration register
}
void ds1820_readalarm()
{
	ds18b20_initial();
	ds18b20_writebyte(0xcc); //Sip ROM command
	ds18b20_writebyte(0xbe);//Read sratchpad command
	ds18b20_readbyte();
	ds18b20_readbyte();
	a=ds18b20_readbyte();
	b=ds18b20_readbyte();
//	ds18b20_initial();
}
void set_limit(uchar cursor)
{
	while(keysix_flag==0)
	{
		keysix_scan();
		keyseven_scan();
		keyeight_scan();
		if(keyseven_flag==1)
		{	
			keyseven_flag=0;
			ds1820_readalarm();
			switch(cursor)
			{
				case 0: a+=1;
						ds18b20_writealarm(a,b);
						ds1820_readalarm();
						lcd_writecommand(0x80+0x46+cursor-1);
						lcd_writedata(decimal_character(a/10));
						lcd_writedata(decimal_character(a%10));
						break;
				case 7: b+=1;
						ds18b20_writealarm(a,b);
						ds1820_readalarm();
						lcd_writecommand(0x80+0x46+cursor-1);
						lcd_writedata(decimal_character(b/10));
						lcd_writedata(decimal_character(b%10));
						break;   
			   default: break;
			}
		}
		if(keyeight_flag==1)
		{	
			keyeight_flag=0;
			ds1820_readalarm();
			switch(cursor)
			{
				case 0: a-=1;
						ds18b20_writealarm(a,b);
						ds1820_readalarm();
						lcd_writecommand(0x80+0x46+cursor-1);
						lcd_writedata(decimal_character(a/10));
						lcd_writedata(decimal_character(a%10));
						break;
				case 7: b-=1;
						ds18b20_writealarm(a,b);
						ds1820_readalarm();
						lcd_writecommand(0x80+0x46+cursor-1);
						lcd_writedata(decimal_character(b/10));
						lcd_writedata(decimal_character(b%10));
						break;   
			   default: break;
			}
		}
	}
}

void shift_cursor()
{
	uchar i,cursor=0;
	while(keysix_flag==1)
	{
		keysix_flag=0;
		for(i=0;i<5;i++)
		{
			lcd_writecommand(0x80+0x46+cursor);
			lcd_writedata('_');
			delayms(500);
			lcd_writecommand(0x80+0x46+cursor);
			lcd_writedata(' ');
			delayms(500);
			keysix_scan();
			keyseven_scan();
			keyeight_scan();
			if(keysix_flag==1)
				break;
			if(keyseven_flag==1||keyeight_flag==1)
			{
				set_limit(cursor);
			    break;
			}
		}
		cursor+=7;
		if(cursor==14)
			cursor=0;
	}



}
void set_alarm()
{
	uchar i;
	keyfive_flag=0;
	lcd_writecommand(0x01);
	ds1820_readalarm();
	while(keyfive_flag==0)
	{
		lcd_writecommand(0x80+0x02);
		for(i=0;i<12;i++)
		{
			lcd_writedata(alarm_panel[i]);
		}
		lcd_writecommand(0x80+0x42);
		for(i=13;i<16;i++)
		{
			lcd_writedata(alarm_panel[i]);
		}
		lcd_writedata(decimal_character(a/10));
		lcd_writedata(decimal_character(a%10));
		lcd_writecommand(0x80+0x49);
		for(i=17;i<20;i++)
		{
			lcd_writedata(alarm_panel[i]);
		}
		lcd_writedata(decimal_character(b/10));
		lcd_writedata(decimal_character(b%10));
		if(keysix_flag==1)
			shift_cursor();
		keyfive_scan();
		keysix_scan();
	}
		ds18b20_initial();
		ds18b20_writebyte(0xcc);
		ds18b20_writebyte(0x48);
}	
