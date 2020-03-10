#include<reg52.h>

sbit PWM = P0^0;//pwm输出引脚，接了个小灯
//38译码器的一个输出引脚控制共阴发光二极管的阴极
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char HIGHh = 0;
unsigned char HIGHl = 0;
unsigned char LOWh = 0;
unsigned char LOWl = 0;

void PWMconfig(unsigned int fre, unsigned char dc);//2 带有形式参数的函数声明需要带上形参。 3 参数形式问题
void PWMstop();

int main(void)//void main()这种写法不符合C语言规范,我也没弄很清楚
{
	unsigned int i;
	
	EA = 1;//4 打开各种开关
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	while(1)//5 没有while就亮一会儿
	{
	PWMconfig(100,40);
	for(i=1000;i>0;i--);//  9 把分号写成了逗号
	PWMstop();
	}
}

void PWMconfig(unsigned int fre,unsigned char dc)
{
	unsigned int th, tl;//6 变量问题没有负数，所以带个unsigned好一些
	unsigned long tmp;
	//char HIGHh, HIGHl, LOWl, LOWh;1 这四个变量需要在其他函数中使用，所以必须是全局变量
	
	//th = 65536 - (1/fre)*(dc/100) / (12/11059200);th的类型是整型，算式中的某一步运算出现了小数，也就是浮点数
	//tl = 65536 - (1/fre)*(1-dc/100) / (12/11059200);然后就那个浮点数就为零了。
	tmp = (11059200/12)/fre;
	th = tmp*dc/100;
	tl = tmp - th;
	th = 65536 - th + 12;
	tl = 65536 - tl + 12;
	HIGHh = (unsigned char)th >> 8;//  7 强制类型转换的问题，好好看看
	HIGHl = (unsigned char)th;
	LOWh = (unsigned char)tl >> 8;
	LOWl = (unsigned char)tl;
	
	TMOD &= 0XF0;//  8 定时器的配置
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
	//中断关
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
	//PWM == 0?....:.....;有点长不适合
}