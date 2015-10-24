#include<reg51.h>
#define uint unsigned int 
#define uchar unsigned char
uchar rom_alarm_code[8],rom_read_code[8];
sbit beep=P1^5;
/*********调用外部函数及变量声明部分**********/
void ds18b20_writebyte(uchar dat);
uchar ds18b20_readbyte();
void ds18b20_initial();

/*********调用外部函数及变量声明部分**********/
static void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
void ds18b20_set_alarm()
{
	ds18b20_initial();		   //ds18b20初始化
	ds18b20_writebyte(0xcc);  //Skip ROM command
	ds18b20_writebyte(0x4e);  //Write Scratchpad command
	ds18b20_writebyte(0x14); //TH设置为20
	ds18b20_writebyte(0x00); //TL设置为0
	ds18b20_writebyte(0x7f); //config设置为12bit分辨率
}
void ds18b20_read_rom()
{
	uchar i;
	ds18b20_initial();
	ds18b20_writebyte(0x33);//Read ROM command
	for(i=0;i<8;i++)
	{
		rom_read_code[i]=ds18b20_readbyte();
	}
}
void ds18b20_search_alarm()
{
	uchar j,flag;
	ds18b20_initial();		    //ds18b20初始化
	ds18b20_writebyte(0xec);  //Alarm search指令
	for(j=0;j<8;j++)
	{
		rom_alarm_code[j]=ds18b20_readbyte();
	}	
	for(j=0,flag=0;j<8;j++)
	{
		if(rom_alarm_code[j]!=rom_read_code[j])
			flag++;	
	}
	if(flag==0)
	{
		while(1)
		{
			beep=~beep;
			delayms(500);	
		}
	}
}




