#include<reg52.h>

unsigned char TH;
unsigned char TL;
sbit buzzer = P1^0;

void stopbuzzer();
void configBuzzer(unsigned int fer);

int main(void)
{
	int i;
	//TMOD = 0X01;
	//EA = 1;  书上写在这里
	while(1)
	{
	/*void*/ configBuzzer(1000);//调用函数的时候就不用写返回值的类型啦。
	for(i=0; i>4000; i++);
	/*void*/ stopbuzzer();
	}
}

void configBuzzer(unsigned int fre)
{
	unsigned int tmp;
	tmp = 65536 - (11059200/12)/(fre*2);
	TH = (unsigned char)(tmp >> 8);
	TL = (unsigned char)tmp;
	
	EA = 1;
	TH0 = TH;//书上赋值写在了中断里，这里它赋了一个即将进入中断一个的值，就是两个0xff
	TL0 = TL;
	TMOD = 0X01;//书上写在main中
	TR0 = 1;
	ET0 = 1;//书上写在main中
	buzzer = 0;
	
}

void Interrupt() interrupt 1
{
	//buzzer ~= buzzer;这个错误。。。
	buzzer =~ buzzer;
}

void stopbuzzer()
{
	ET0 = 0;
	TR0 = 0;
	buzzer = 1;
}