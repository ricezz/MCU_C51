#include<reg51.h>
unsigned char i=0;
unsigned int x,num=0;

unsigned char xy[]={0xf9, 0x80, 0xc0, 0xb0, 0xc0, 0xf8, 0xa4, 0x90};

void delay(int a);
void config(unsigned int baud);

void main()
{
	int m=0x01;
	TMOD = 0X01;
	TH0 = 0X3C;
	TL0 = 0XB0;
	TR0 = 0;
	EA = 1;
	
	
	config(4800);
	SBUF = xy[i];
	while(1)
	{
		if(TI==1)
		{
			for(x=0;x<20;x++)//定时器0，查询方式，延迟一秒
			  {
				TH0=0x3c;
				TL0=0xb0;
				TR0=1;
				while(!TF0);
				TF0=0;
			  }		
			  
				i += 1;
				if(i==8)i=0;
			  SBUF = xy[i];//发送数据
				TI = 0;
				num=0;			
		}
	}
}

void config(unsigned int baud)
{
	SCON = 0x50;//串口模式一
	TMOD |= 0x20;//按位或运算，只改变定时器一的模式，不改变定时器零的模式
	TH1 = 256 - (11059200/12/32)/baud;  //计算 T1 重载值
	TL1 = TH1;
	ET1 = 0;
	ES = 0;
	TR1 = 1;
}

