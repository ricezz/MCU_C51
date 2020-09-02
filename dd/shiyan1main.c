#include<reg51.h>//晶振频率12M

unsigned char b = 0;
unsigned int cnt = 0;

unsigned  char code duan[] = {0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 
							  0xf9, 0x80, 0xc0, 0xb0, 0xc0, 0xf8, 0xa4, 0x90,//这里表示你的学号18030729
							  0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf,0xbf};
unsigned char wei[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//表示1-8

void delay();//声明delay函数

void main()
{
	unsigned char a ;
	unsigned char j;
	int i = 800;
	
	TMOD = 0X01;//定时器0模式1；
	TH0 = 0xfc;//设定初值，1ms进入一次中断
	TL0 = 0x18;
	TR0 = 0;//关闭定时器0
	ET0 = 1;//使能定时器0中断
	EA = 1;//打开总中断
	while(i--)//静态显示学号5秒钟
	{
		for(j=0;j<8;j++)//静态显示学号
		{
			P0 = duan[7+j];
			P1 = wei[j];
			delay();
			P0 = 0xff;
		}
		if(i==1)//静态显示将要结束时，打开定时器0
		{
			TR0 = 1;
		}
	}
	
	while(1)//滚动显示学号
	{
		for(a=0;a<8;a++)
		{
			P0 = duan[a+b];
			P1 = wei[a];
			delay();
			P0 = 0XFF;
		}
	}
 
}

void interrupt1() interrupt 1//定时器0中断，目的：每隔一秒变量b加一
{
	TH0 = 0xfc;
	TL0 = 0x18;
		cnt +=1;
			
			if(cnt>1000)
			{
				cnt = 0;
				b += 1;
				if(b>15)
				{
					b=0;
				}
			}
}

void delay()//延迟函数
{
	int i=100;
	while(i--);
}