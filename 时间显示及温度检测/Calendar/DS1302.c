#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
/************************
*  DS1302功能引脚位定义
*************************/
sbit ds1302data=P3^4; //ds1302数据通信线
sbit ds1302rst=P3^5;  //ds1302复位信号
sbit ds1302sclk=P3^6;  //ds1302同步时钟信号
/*********************************************
   定义 DS1302 clock/calendar 寄存器初始化值
   读、写 DS1302 clock/calendar 寄存器的地址
**********************************************/
uchar code ds1302_initialcalendar[]={0,0,12,11,10,4,12}; //2012年10月8日星期一12：00
uchar code ds1302_write_ckaddress[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //写clock/calendar地址
uchar code ds1302_read_ckaddress[]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  //读clock/calendar地址
/*************************************************
*  函数功能：向ds1302写一个字节内容（地址或数据）
*  入口参数：uchar content
*  出口参数：
**************************************************/
void ds1302_writebyte(uchar content) 
{
	uchar i;
	ds1302sclk=0;
	_nop_();
	ds1302rst=1;
	for(i=0;i<8;i++)
	{		
		ds1302sclk=0;
		_nop_();
		ds1302data=content&0x01;
		_nop_();
		ds1302sclk=1;		//每次写完后保持ds1302sclk为高电平状态
		content>>=1;
	}
}
/************************************************************
*  函数功能：向ds1302先写一个字节地址，然后再写一个字节数据
*  入口参数：uchar address,uchar dat
*  出口参数：
*************************************************************/
void ds1302_write(uchar address,uchar dat)
{
	ds1302rst=0;
	ds1302_writebyte(address);
	ds1302_writebyte(dat);
	ds1302rst=0;
}
/*************************************
*  函数功能：从ds1302读出一个字节数据
*  入口参数：
*  出口参数：uchar dat
**************************************/
uchar ds1302_readbyte()
{
	uchar i,dat=0;
	for(i=0;i<8;i++)
	{
		ds1302sclk=0;	
	 	dat>>=1;
		if(ds1302data)
			dat|=0x80;
		ds1302sclk=1;
		_nop_();
	}
	return dat;
}
/**********************************
*  函数功能：从ds1302读出一个字节
*  入口参数：uchar address
*  出口参数：uchar dat
***********************************/
uchar ds1302_read(uchar address)
{
	
	uchar dat;
	ds1302rst=0;
	ds1302_writebyte(address);
	dat=ds1302_readbyte();
	ds1302rst=0;
	return dat;
}
/***********************************
*  函数功能：将十进制码转换成BCD码
*  入口参数：uchar decimal
*  出口参数：
************************************/
uchar decimal_bcd(uchar decimal)
{
	return((decimal/10)*16+(decimal%10)); //运算符的优先级！！！
}
/*******************************************
*  函数功能：将BCD码十进制码转换成十进制码
*  入口参数：uchar bcd
*  出口参数：
*******************************************/
uchar bcd_decimal(uchar bcd)		 ////运算符的优先级！！！
{
	return((bcd/16)*10+(bcd&0x0f));

}
/********************************
*  函数功能：对ds1302进行初始化
*  入口参数：
*  出口参数：
********************************/
void ds1302_initial()
{
	uchar i;
	ds1302_write(0x8e,0x00); //写Control寄存器地址/命令字节,将Control寄存器的最高位wp置零														  
	for(i=0;i<7;i++)
	{
		ds1302_write(ds1302_write_ckaddress[i],decimal_bcd(ds1302_initialcalendar[i]));
		_nop_();
	}
}
	
	
	
	
	

	




