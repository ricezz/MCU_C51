#include<reg52.h>

sbit PWM = P0^0;//pwm������ţ����˸�С��
//38��������һ��������ſ��ƹ�����������ܵ�����
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char HIGHh = 0;
unsigned char HIGHl = 0;
unsigned char LOWh = 0;
unsigned char LOWl = 0;

void PWMconfig(unsigned int fre, unsigned char dc);//2 ������ʽ�����ĺ���������Ҫ�����βΡ� 3 ������ʽ����
void PWMstop();

int main(void)//void main()����д��������C���Թ淶,��ҲûŪ�����
{
	unsigned int i;
	
	EA = 1;//4 �򿪸��ֿ���
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	while(1)//5 û��while����һ���
	{
	PWMconfig(100,40);
	for(i=1000;i>0;i--);//  9 �ѷֺ�д���˶���
	PWMstop();
	}
}

void PWMconfig(unsigned int fre,unsigned char dc)
{
	unsigned int th, tl;//6 ��������û�и��������Դ���unsigned��һЩ
	unsigned long tmp;
	//char HIGHh, HIGHl, LOWl, LOWh;1 ���ĸ�������Ҫ������������ʹ�ã����Ա�����ȫ�ֱ���
	
	//th = 65536 - (1/fre)*(dc/100) / (12/11059200);th�����������ͣ���ʽ�е�ĳһ�����������С����Ҳ���Ǹ�����
	//tl = 65536 - (1/fre)*(1-dc/100) / (12/11059200);Ȼ����Ǹ���������Ϊ���ˡ�
	tmp = (11059200/12)/fre;
	th = tmp*dc/100;
	tl = tmp - th;
	th = 65536 - th + 12;
	tl = 65536 - tl + 12;
	HIGHh = (unsigned char)th >> 8;//  7 ǿ������ת�������⣬�úÿ���
	HIGHl = (unsigned char)th;
	LOWh = (unsigned char)tl >> 8;
	LOWl = (unsigned char)tl;
	
	TMOD &= 0XF0;//  8 ��ʱ��������
	TMOD |= 0X01;
	th = HIGHh;
	tl = HIGHl;
	ET0 = 1;
	TR0 = 1;
	PWM = 1;
}

void PWMstop()
{
	TR0 = 0;
	ET0 = 0;
	PWM = 1;
	//�жϹ�
}

void InterruptTmer0() interrupt 1
{
	if(PWM == 0)
	{
		PWM = 1;
		TH0 = HIGHh;
		TL0 = HIGHl;
	}
	else
	{
		PWM = 0;
		TH0 = LOWh;
		TL0 = LOWl;
	}
	//PWM == 0?....:.....;�е㳤���ʺ�
}