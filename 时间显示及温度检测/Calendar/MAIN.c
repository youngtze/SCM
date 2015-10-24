#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit beep=P1^5;
uchar ds1302_display[]="20  -  -    :  :  ";  //记录年月日 时分秒信息
uchar ds1302_day[][3]={{'M','O','N'},{'T','U','E'},{'W','E','N'},{'T','H','U'},{'F','R','I'},{'S','A','T'},{'S','U','N'}};	  //依次从星期一到星期日
uchar ds18b20_temp_tip[]="Temp has exceed!";
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
/*************************************
*  函数功能：将十进制码转换成ASCII码
*  入口参数：uchar decimal
*  出口参数：
*************************************/
uchar decimal_character(uchar decimal)
{
	return(decimal+48);
}	
int main()
{	
/***********调用外部函数及变量声明部分****************/
	void lcd_initial();
	uchar bcd_decimal(uchar bcd);
	void lcd_writecommand(uchar command);
	void lcd_writedata(uchar dat);
	void ds1302_initial();
	uchar ds1302_read(uchar address);
	void ds1302_write(uchar address,uchar dat);
	void ds18b20_convert();
	uint ds18b20_deal();
//	void ds18b20_set_alarm();
//	void ds18b20_read_rom();
//	void ds18b20_search_alarm();
	void keyone_scan();
	void keyfive_scan();
	void set_cursor();
	void set_alarm();
	void ds1820_readalarm();
	extern uchar code ds1302_write_ckaddress[];
	extern uchar code ds1302_read_ckaddress[];
	extern bit keyone_flag;
	extern bit keyfive_flag;
	extern uchar a;
	extern uchar b;
/***********调用外部函数及变量声明部分****************/
	uchar k,m,n;
	lcd_initial();
	ds1302_initial();

	while(1)
	{	
			keyone_scan();
			keyfive_scan();
//			keytwo_scan();
			if(keyone_flag==1)
			{
				ds1302_write(ds1302_write_ckaddress[0],0x80|ds1302_read(ds1302_read_ckaddress[0]));	 //如果检测到K1键被按下，则向Second寄存器的最高位写1，进入standby工作模式
				set_cursor();
				keyone_flag=0;
				ds1302_write(ds1302_write_ckaddress[0],0x7f&ds1302_read(ds1302_read_ckaddress[0]));	//将Second寄存器的最高位写0，进入start工作模式
			}
			if(keyfive_flag==1)
			{
				set_alarm();
				lcd_initial();
				keyfive_flag=0;  
			}
				
/********************从DS1302中读出当前的时间数据并转化成ASCII码存储在数组中******************************/
			ds1302_display[17]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))%10);  //秒
			ds1302_display[16]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))/10);
			ds1302_display[14]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))%10);  //分
			ds1302_display[13]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))/10);
			ds1302_display[11]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))%10);  //时
			ds1302_display[10]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))/10);
			ds1302_display[9]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))%10);	  //日
			ds1302_display[8]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))/10);
			ds1302_display[6]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))%10);	  //月
			ds1302_display[5]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))/10);						
			ds1302_display[3]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))%10);	  //年
			ds1302_display[2]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))/10);
/*********************从DS1302中读出当前的时间数据并转化成ASCII码存储在数组中******************************/

/************将从clock/calendar寄存器中读出的时间数据依次写入LCD1602***********************/
		lcd_writecommand(0x80);	 
		for(k=0;k<10;k++)
		{
			lcd_writedata(ds1302_display[k]);
		}
		lcd_writecommand(0x80+0x40);
		for(m=10;m<18;m++)
		{
			lcd_writedata(ds1302_display[m]);
		}
/************将从clock/calendar寄存器中读出的时间数据依次写入LCD1602***********************/

        lcd_writecommand(0x80+0x0c);
		for(n=0;n<3;n++)
		{
			lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][n]);	//将day信息写入LCD1602 
		}
		
/************将从ds18b20中读出的数据依次写入LCD1602***************/
		ds18b20_convert();			 //转换完毕后，ds18b20将进入idle state,所以需要反复开启
		
		lcd_writecommand(0x80+0x4c);
		lcd_writedata(decimal_character(ds18b20_deal()/100));
		lcd_writedata(decimal_character(ds18b20_deal()/10%10));
		lcd_writedata(46);
		lcd_writedata(decimal_character(ds18b20_deal()%10));
/************将从ds18b20中读出的数据依次写入LCD1602***************/
	
/*************DS13B20检测温度是否超过设定值*********************/
		ds1820_readalarm();
		if((ds18b20_deal())>(a*10)||(ds18b20_deal())<(b*10))
		{
			uchar i;
//			delayms(30);
			lcd_writecommand(0x01);	 //清屏
			lcd_writecommand(0x80);
			for(i=0;i<16;i++)
			{
				lcd_writedata(ds18b20_temp_tip[i]);
			}
			lcd_writecommand(0x80+0x46); 
			lcd_writedata(decimal_character(ds18b20_deal()/100));
			lcd_writedata(decimal_character(ds18b20_deal()/10%10));
			beep=~beep;
			lcd_writedata(46);
			lcd_writedata(decimal_character(ds18b20_deal()%10));
			delayms(5000);
			lcd_writecommand(0x01);;	 //清屏
		}
	}
	return 0;
}


			
				
			
			
		
			
		
		