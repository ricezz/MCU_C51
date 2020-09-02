#include<reg51.h>//����Ƶ��12M

unsigned char b = 0;
unsigned int cnt = 0;

unsigned  char code duan[] = {0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 
							  0xf9, 0x80, 0xc0, 0xb0, 0xc0, 0xf8, 0xa4, 0x90,//�����ʾ���ѧ��18030729
							  0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf,0xbf};
unsigned char wei[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//��ʾ1-8

void delay();//����delay����

void main()
{
	unsigned char a ;
	unsigned char j;
	int i = 800;
	
	TMOD = 0X01;//��ʱ��0ģʽ1��
	TH0 = 0xfc;//�趨��ֵ��1ms����һ���ж�
	TL0 = 0x18;
	TR0 = 0;//�رն�ʱ��0
	ET0 = 1;//ʹ�ܶ�ʱ��0�ж�
	EA = 1;//�����ж�
	while(i--)//��̬��ʾѧ��5����
	{
		for(j=0;j<8;j++)//��̬��ʾѧ��
		{
			P0 = duan[7+j];
			P1 = wei[j];
			delay();
			P0 = 0xff;
		}
		if(i==1)//��̬��ʾ��Ҫ����ʱ���򿪶�ʱ��0
		{
			TR0 = 1;
		}
	}
	
	while(1)//������ʾѧ��
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

void interrupt1() interrupt 1//��ʱ��0�жϣ�Ŀ�ģ�ÿ��һ�����b��һ
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

void delay()//�ӳٺ���
{
	int i=100;
	while(i--);
}