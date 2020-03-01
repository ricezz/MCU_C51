#include<reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

sbit KEY_IN_1 = P2^4;
sbit KEY_IN_2 = P2^5;
sbit KEY_IN_3 = P2^6;
sbit KEY_IN_4 = P2^7;
sbit KEY_OUT_1 = P2^3;
sbit KEY_OUT_2 = P2^2;
sbit KEY_OUT_3 = P2^1;
sbit KEY_OUT_4 = P2^0;

unsigned char code Ledchar[] = {//code的作用见onenote，单片机，矩阵键盘。
	0xc0, 0xf9, 0xa4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E 
};

unsigned char KeySta[4][4] = { //这里用char字符型我觉是因为省空间，它只占一个字节。
 {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

int main()
{
	unsigned char i, j;
	unsigned char backup[4][4] = {
		{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}
	};
	
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	EA = 1;
	TMOD = 0x01;
	TH0 = 0xfc;
	TL0 = 0x67;
	ET0 = 1;
	TR0 = 1;
	P0 = Ledchar[0];
	
	while(1)
	{
		for(i=0; i<4; i++)//两个for循环把矩阵按键扫描一遍
		{
			for(j=0; j<4; j++)
			{
				if(backup[i][j] != KeySta[i][j])//两个判断，一个检查按键是否变化，按下进入，抬起也进入
				{//另一个检查是按下动作还是抬起动作
					if(backup[i][j]!=0)//按下动作不进入复合句中，只有抬起才进入。两个if语句的配合使按键经过按下和弹起两个动作后数值才变化。
					{
						P0 = Ledchar[i*4+j];
					}
					backup[i][j] = KeySta[i][j];//每当有按键变化就会更改备份的值，但是若按下而不抬起第一个if语句都进不去
				}
					
			}
		}
	}
}

/*T0中断，扫描按键的状态，消抖。*/
void InterruptTimer0() interrupt 1
{
	unsigned char i;
	static unsigned char keyout = 0;//静态局部变量，每次调用继续用上次的值，不会初始化
	static unsigned char keybuf[4][4] = {//!!!在这一行和上面一行犯两个错误，第一个：unsigned写成unsiged；第二个：unsigned写成unaigned。
		 {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}, 
		 {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF} 
	};
	TH0 = 0xfc;
	TL0 = 0x67;
	//以上，定义变量和数组，定义初值。
	keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
	keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
	keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
	keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
	
	for(i=0; i<4; i++)//检查四个按键的缓存
	{
		if((keybuf[keyout][i]&0x0f) == 0x00)//!!!按位与需要加括号，运算顺序很重要啊。       在这犯的第二个错误==写成=。
		{
			KeySta[keyout][i] = 0;
		}
		else if((keybuf[keyout][i]&0x0f) == 0x0f)//！！！加括号。==是等号，=是赋值运算符
		{
			KeySta[keyout][i] = 1;
		}
		//if    else if的第二种写法
		/*
		keybuf[keyout][i]&0x0f = 0x00 ? KeySta[keyout][i]=0 : keybuf[keyout][i]&0x0f = 0x0f ? KeySta[keyout][i] = 1 :{} ; 不知道对不对  好像写反了
		多个三目运算符的顺序是从右到左，单个是从左到右
		不知道这个能不能换行
		太难用了
		*/
	}
	keyout++;
	keyout = keyout&0x03;//这个细节有意思
	switch (keyout)//这个也要好好想想，书上说这样保证信号的稳定性，我有点不明白
	{//一个KEY_OUT是高电平，其余应该都是高电平
		case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
		case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
		case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
		case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
		default:  break;
	}
}

