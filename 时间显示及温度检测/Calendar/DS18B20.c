#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
/************************
*  DS18B20功能引脚位定义
*************************/
sbit ds18b20=P3^7;	 //ds18b20单总线
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
/***************************************
*  函数功能：向ds18b20写入一个字节数据
*  入口参数：uchar dat
*  出口参数：
****************************************/
void ds18b20_writebyte(uchar dat)
{
	uchar i,j;
	for(i=0;i<8;i++)
	{
//		ds18b20=1;
//		NOP;
		ds18b20=0;
		_nop_();
		_nop_();
		ds18b20=0x01&dat;
		for(j=10;j>0;j--);
		ds18b20=1;		   //拉高总线回到 idle state
		dat>>=1;
	}
}
/**************************************
*  函数功能：从ds18b20读出一个字节数据
*  入口参数：
*  出口参数：uchar dat
***************************************/
uchar ds18b20_readbyte()
{
	uchar i,j,dat=0;
	for(i=0;i<8;i++)
	{
//		ds18b20=1;
//		NOP;
		ds18b20=0;
		_nop_();
		dat>>=1;
		ds18b20=1;
		if(ds18b20)
			dat|=0x80;
		for(j=8;j>0;j--);
		ds18b20=1;		 //拉高总线回到 idle state
//		NOP;
	}
	return dat;
}
/*********************************
*  函数功能：对ds18b20进行初始化
*  入口参数：
*  出口参数：
**********************************/
void ds18b20_initial()
{
	uint i,j,k=10;
	ds18b20=1;
	_nop_();
	ds18b20=0;
	for(i=80;i>0;i--); //主机拉低总线至少480us
	ds18b20=1;		   //主机释放总线
	for(j=10;j>0;j--);
	while(ds18b20&&k--);
	for(i=70;i>0;i--);
	ds18b20=1;
}
/******************************************************
*  函数功能：从ds18b20读取转换后的温度，并进行相关处理
*  入口参数：
*  出口参数：
*******************************************************/
uint ds18b20_deal()
{
	uint a,b,c;
	float temp;
	ds18b20_initial();
	delayms(1);
	ds18b20_writebyte(0xcc);  //写跳过ROM指令
//	delayms(1);
	ds18b20_writebyte(0xbe);  //写读取温度指令
//	delayms(1);
	a=ds18b20_readbyte();	 //读取低8位温度
//	delayms(1);
	b=ds18b20_readbyte();   //读取高8位温度
	b<<=8;
	b|=a;
	temp=b*0.0625; //温度转换，分辨率为12位（0.0625）
	c=temp*10+0.5; //保留一位小数，并进行四舍五入处理
	return c;
}
/**************************************
*  函数功能：对ds18b20开启温度转换
*  入口参数：
*  出口参数：
***************************************/
void ds18b20_convert()
{
		ds18b20_initial();			//初始化
//		delayms(1);
		ds18b20_writebyte(0xcc);	//跳过ROM命令
//		delayms(1);
		ds18b20_writebyte(0x44);   //开启温度转换
		delayms(700);	  //等待温度转换完成
}


