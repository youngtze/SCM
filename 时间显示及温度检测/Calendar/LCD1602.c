#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
/************************
*  LCD1602功能引脚位定义
*************************/
sbit lcden=P2^7;  //lcd1602选信号
sbit lcdrs=P2^6;  //lcd1602数据/命令信号
sbit lcdrw=P2^5;  //lcd1602读写信号
/******************************
*  函数功能：软件延时（约1ms）
* 入口参数：uint xms
*  出口参数：
*******************************/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
/*********************************
*  函数功能：向LCD写一个字节命令
*  入口参数：uchar command
*  出口参数：
**********************************/
void lcd_writecommand(uchar command)
{
	lcdrs=0;
	lcdrw=0;
	lcden=0;
	P0=command;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}
/********************************
*  函数功能：向LCD写一个字节数据 
*  入口参数：uchar dat
*  出口参数：
*********************************/
void lcd_writedata(uchar dat)
{
	lcdrs=1;
	lcdrw=0;
	lcden=0;
	P0=dat;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}
/***************************
*  函数功能：对LCD进行初始化 
*  入口参数：
*  出口参数：
****************************/
void lcd_initial()
{
//	delayms(15);
//	lcd_writecommand(0x38);
//	delayms(5);
//	lcd_writecommand(0x38);
//	delayms(5);
	lcd_writecommand(0x38);	  //显示模式设置 16x2显示 5x7点阵 8位数据接口
	delayms(10);
	lcd_writecommand(0x08);	  //关显示
	delayms(10);
	lcd_writecommand(0x01);	  //显示清屏 （数据指钟清零，所有显示清零）
	delayms(10);
	lcd_writecommand(0x06);	  //写一个字符后地址指针加一，且光标加一
	delayms(10);
	lcd_writecommand(0x0c);	  //开显示，不显示光标，不闪烁
}

	
		