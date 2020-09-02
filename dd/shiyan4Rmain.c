#include<reg51.h>

bit v=0;
unsigned char i=0;
unsigned char R=0;
unsigned char xyz[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char w[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

void delay(int a);
void config(unsigned int baud);

void main()
{
	unsigned char l,q;
	unsigned char j=0;
	
	EX0 = 1;
	IT0 = 1;
	
	EA = 1;
	config(4800);//配置串口通信

	while(1)
	{
		if(RI)
		{
			xyz[i]=SBUF;
			if(v==0)P2=0x80;P0=xyz[i];//最后一个数码管显示数字
			i += 1;
			if(i==8)i=0;
			RI=0;
		}
		if(v==1)
		{
			for(l=0;l<8;l++)//八位数码管显示数字
			{
				P2 = w[l];
				P0 = xyz[7-l];
				for(q=0;q<200;q++);
				P2 = 0X00;
			}
		}
	}

}


void tupt0() interrupt 0
{
	v=~v;
}

void config(unsigned int baud)
{
	SCON = 0x50;//串口模式一
	TMOD = 0x20;
	TH1 = 256 - (11059200/12/32)/baud;  //计算 T1 重载值
	TL1 = TH1;
	ET1 = 0;
	TR1 = 1;
}