#include<reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit keyone=P1^0;
sbit keytwo=P1^1;
sbit keythree=P1^2;
bit keyone_flag=0,keytwo_flag=0,keythree_flag=0;
/********调用外部函数及变量声明部分*************/
void lcd_writecommand(uchar command);
void lcd_writedata(uchar dat);
uchar ds1302_read(uchar address);
void ds1302_write(uchar address,uchar dat);
uchar bcd_decimal(uchar bcd);
uchar decimal_bcd(uchar decimal);
uchar decimal_character(uchar decimal);
extern uchar code ds1302_write_ckaddress[];
extern uchar code ds1302_read_ckaddress[];
extern uchar ds1302_display[];
extern uchar ds1302_day[][3];
/********调用外部函数及变量声明部分*************/

static void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
/***************按键检测程序****************/
void keyone_scan()
{
	if(keyone==0)
	{
		delayms(10);
		if(keyone==0)
		{	
			while(keyone==0);
			keyone_flag=1;
		}
	}
}
void keytwo_scan()
{
	if(keytwo==0)
	{
		delayms(10);
		if(keytwo==0)
		{	
			while(keytwo==0);
			keytwo_flag=1;
		}
	}
}
void keythree_scan()
{
	if(keythree==0)
	{
		delayms(10);
		if(keythree==0)
		{	
			while(keythree==0);
			keythree_flag=1;
		}
	}
}
/***************按键检测程序****************/
void set_calendar(uchar count,uchar cursor)
{
	while(keyone_flag==0)
	{
		keyone_scan();
		keytwo_scan();
		keythree_scan();
/************************日历调整加1操作****************************/
		if(keytwo_flag==1)
		{
			keytwo_flag=0;
			switch(count)
			{
				case 0:if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))+1>99)	  //从year寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[6],0);
						else
							ds1302_write(ds1302_write_ckaddress[6],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))+1));
						ds1302_display[3]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))%10);	  
						ds1302_display[2]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[2]);
						lcd_writedata(ds1302_display[3]);
						break;
				case 1:	if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))+1>12)	  //从month寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[4],1);
						else
							ds1302_write(ds1302_write_ckaddress[4],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))+1));
						ds1302_display[6]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))%10);	 
						ds1302_display[5]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[5]);
						lcd_writedata(ds1302_display[6]);
						break;	
				case 2: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))+1>31)	  //从date寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[3],1);
						else
							ds1302_write(ds1302_write_ckaddress[3],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))+1));
						ds1302_display[9]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))%10);	  
						ds1302_display[8]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))/10); 
			    		lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[8]);
						lcd_writedata(ds1302_display[9]);
						break;
				case 3:	if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))+1>7)	  //从day寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[5],1);
						else
							ds1302_write(ds1302_write_ckaddress[5],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))+1));
						lcd_writecommand(0x80+cursor-2);
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][0]);  
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][1]);
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][2]);
						break;
				case 4: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))+1>23)	  //从hour寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[2],0);
						else
							ds1302_write(ds1302_write_ckaddress[2],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))+1));
						ds1302_display[11]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))%10);  
						ds1302_display[10]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[10]);
						lcd_writedata(ds1302_display[11]);
						break;
				case 5: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))+1>59)	 //从minute寄存器读出数据后执行加1操作
					   		ds1302_write(ds1302_write_ckaddress[1],0);
						else
							ds1302_write(ds1302_write_ckaddress[1],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))+1));
						ds1302_display[14]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))%10);  
						ds1302_display[13]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[13]);
						lcd_writedata(ds1302_display[14]);
						break;
				case 6:	ds1302_write(ds1302_write_ckaddress[0],0);
						ds1302_display[17]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))%10);  //将second寄存器置0
						ds1302_display[16]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[16]);
						lcd_writedata(ds1302_display[17]);
						break;	
				default: break;
			}	
		}
/************************日历调整加1操作****************************/
	
/************************日历调整减1操作****************************/
		if(keythree_flag==1)
		{
			keythree_flag=0;
			switch(count)
			{
				case 0:if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))==0)	  //从year寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[6],decimal_bcd(99));
						else
							ds1302_write(ds1302_write_ckaddress[6],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))-1));
						ds1302_display[3]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))%10);	  
						ds1302_display[2]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[2]);
						lcd_writedata(ds1302_display[3]);
						break;
				case 1:	if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))-1<1)	  //从month寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[4],decimal_bcd(23));
						else
							ds1302_write(ds1302_write_ckaddress[4],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))-1));
						ds1302_display[6]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))%10);	 
						ds1302_display[5]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[5]);
						lcd_writedata(ds1302_display[6]);
						break;	
				case 2: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))-1<1)	  //从date寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[3],decimal_bcd(31));
						else
							ds1302_write(ds1302_write_ckaddress[3],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))-1));
						ds1302_display[9]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))%10);	  
						ds1302_display[8]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))/10); 
			    		lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[8]);
						lcd_writedata(ds1302_display[9]);
						break;
				case 3:	if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1<1)	  //从day寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[5],decimal_bcd(7));
						else
							ds1302_write(ds1302_write_ckaddress[5],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1));
						lcd_writecommand(0x80+cursor-2);
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][0]);  
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][1]);
						lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][2]);
						break;
				case 4: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))==0)	  //从hour寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[2],decimal_bcd(23));
						else
							ds1302_write(ds1302_write_ckaddress[2],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))-1));
						ds1302_display[11]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))%10);  
						ds1302_display[10]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[10]);
						lcd_writedata(ds1302_display[11]);
						break;
				case 5: if(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))==0)	 //从minute寄存器读出数据后执行减1操作
					   		ds1302_write(ds1302_write_ckaddress[1],decimal_bcd(59));
						else
							ds1302_write(ds1302_write_ckaddress[1],decimal_bcd(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))-1));
						ds1302_display[14]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))%10);  
						ds1302_display[13]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[13]);
						lcd_writedata(ds1302_display[14]);
						break;
				case 6:	ds1302_write(ds1302_write_ckaddress[0],0);
						ds1302_display[17]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))%10);  //将second寄存器置0
						ds1302_display[16]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))/10);
						lcd_writecommand(0x80+cursor-1);
						lcd_writedata(ds1302_display[16]);
						lcd_writedata(ds1302_display[17]);
						break;	
			   default: break;
			}	
		}
/************************日历调整减1操作****************************/

	}			   
} 
void set_cursor()
{
	uchar i,count=0,cursor=0;
	while(keyone_flag==1)
	{		
	    keyone_flag=0;
		cursor+=3;	
/*		lcd_writecommand(0x38);
		lcd_writecommand(0x0c);
		lcd_writecommand(0x06);		  可以去掉   */
		for(i=0;i<5;i++)
		{
			lcd_writecommand(0x80+cursor);
			lcd_writedata('_');
			delayms(500);
			lcd_writecommand(0x80+cursor);
			lcd_writedata(' ');
			delayms(500);
			keyone_scan();
			keytwo_scan();
			keythree_scan();
			if(keyone_flag==1)
				break;	
			if(keytwo_flag==1||keythree_flag==1)
			{
				set_calendar(count,cursor);
				break;
			}
//			if(keythree_flag==1)
//			{
//				set_calendar(count,cursor);
//				break;
//			}
		}
		switch(count)
		{
			case 0:	ds1302_display[3]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))%10);	  //光标消失后写入年信息
					ds1302_display[2]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))/10);
					lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[2]);
					lcd_writedata(ds1302_display[3]);
					break;
			case 1: ds1302_display[6]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))%10);	 //光标消失后写入月信息
					ds1302_display[5]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))/10);
					lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[5]);
					lcd_writedata(ds1302_display[6]);
					break;
			case 2:	ds1302_display[9]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))%10);	  //光标消失后写入日信息
					ds1302_display[8]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))/10); 
		    		lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[8]);
					lcd_writedata(ds1302_display[9]);
					break;
			case 3:	lcd_writecommand(0x80+cursor-2);
					lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][0]);  //光标消失后写入day信息
					lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][1]);
					lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][2]);
					break;
			case 4:	ds1302_display[11]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))%10);  //光标消失后写入时信息
					ds1302_display[10]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))/10);
					lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[10]);
					lcd_writedata(ds1302_display[11]);
					break;
			case 5:	ds1302_display[14]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))%10);  //光标消失后写入分信息
					ds1302_display[13]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))/10);
					lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[13]);
					lcd_writedata(ds1302_display[14]);
					break;
			case 6: ds1302_display[17]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))%10);  //光标消失后写入秒信息
					ds1302_display[16]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))/10);
					lcd_writecommand(0x80+cursor-1);
					lcd_writedata(ds1302_display[16]);
					lcd_writedata(ds1302_display[17]);
					break;
		  default:	break;
		}
		count++;
		if(count==7)
			count=0;
		switch(cursor)
		{
			case 0x09:cursor=11;
					 	break;
			case 0x0e:cursor=0x3e;
						break;
			case 0x47:cursor=0;
						break;
			  default:	break;
		}	 
	}
}


