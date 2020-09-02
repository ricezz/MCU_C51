#include<reg51.h>
#include<intrins.h>
unsigned char c=0;
unsigned char m=0x80;
unsigned char b=1;
unsigned char cnt=0;

sbit key1 = P3^6;
sbit key2 = P3^7;

void delay(int a);

void main()
{
	P0 = 0x80;
	TMOD = 0x11;//设置定时器0模式1，定时器1模式1
	TH0 = 0x3c;//设置定时器0的初值，50ms
	TL0 = 0xb0;
	TR0 = 1;//打开定时器
	ET0 = 1;//打开定时器中断
	
	TH1 = 0xec;//设定定时器1初值，1ms
	TL1 = 0x78;
	TR1 = 1;//打开定时器1
	ET1 = 1;//打开定时器1的中断
	
	IT0 = 1;//设定以下降沿信号触发外部中断0
	EX0 = 1;//打开外部中断0
	
	IT1 = 1;//设定以下降沿信号触发外部中断1
	EX1 = 1;//打开外部中断1
	
	EA = 1;	//打开总中断
	
	while(1)
	{
//		P3=0xff;//37到50行，检测按键是否按下
//		if(P3!=0xff)
//		{
//			delay(100);
//			{
//				if(P3!=0xff)
//				{
//					if(P3==0xbf)
//						c=0;
//					if(P3==0x7f)
//						c=1;
//				}
//			}
//		}
//		P3=0xcc;
//			if (P3!=0xcc)
//			{
//				switch(P3)
//				{
//					case(0X8c):	c=0;break;
//					case(0X4c):	c=1;break;
//				}
//			}
		if(key1==0)c=0;
		if(key2==0)c=1;
		while(key1==0);
		while(key2==0);
		
		if(c==0)//52到83改变流水灯方向
		{
			if(cnt>20)
			{
				m = m>>1;				
				P0 = m;
				cnt = 0;
				if(m==0)
				{
					m=0x80;
					P0 = m;
				}
			}
		}
		else if(c==1)
		{
			if(cnt>20)
			{
				m = m<<1;				
				P0 = m;
				cnt = 0;
				if(m==0)
				{
					m=0x01;
					P0 = m;
				}
			}
		}
	}
}

void delay(int a)
{
	unsigned char i, j;
	for(i=0;i<a;i++)
	{
		for(j=0;j<125;j++);
	}
}


void rupt1() interrupt 1//定时器0中断函数，计时用的，每50ms，cnt加一
{
	TH0 = 0x3c;
	TL0 = 0xb0;
	cnt +=1;
}


void rupt3() interrupt 3//定时器1中断函数，扫描P3.6P3.7那两个按键是否被按下
{
	TH1 = 0xec;
	TL1 = 0x78;
	if(key1==0)
	{
		c = 0;
	}
	else if(key2==0)
	{
		c = 1;
	}
}


void rupt0() interrupt 0//外部中断0，让流水灯左移
{
	c = 0;
}


void rupt2() interrupt 2//外部中断1，让流水灯右移
{
	c = 1;
}