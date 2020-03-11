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
	//EA = 1;  ����д������
	while(1)
	{
	/*void*/ configBuzzer(1000);//���ú�����ʱ��Ͳ���д����ֵ����������
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
	TH0 = TH;//���ϸ�ֵд�����ж������������һ�����������ж�һ����ֵ����������0xff
	TL0 = TL;
	TMOD = 0X01;//����д��main��
	TR0 = 1;
	ET0 = 1;//����д��main��
	buzzer = 0;
	
}

void Interrupt() interrupt 1
{
	//buzzer ~= buzzer;������󡣡���
	buzzer =~ buzzer;
}

void stopbuzzer()
{
	ET0 = 0;
	TR0 = 0;
	buzzer = 1;
}