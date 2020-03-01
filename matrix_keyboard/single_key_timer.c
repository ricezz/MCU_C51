#include<reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY4 = P2^7;

unsigned char code LedChar[] = {
0xc0, 0xF9, 0xA4, 0xB0, 0X99, 0x92, 0x82, 0xf8,
0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};
bit KeySta = 1;
void main()
{
	bit backup = 1;
	unsigned char cnt = 0;
	
	EA = 1;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	TMOD = 0X01;
	TH0 = 0xF8;
	TL0 = 0xCD;
	ET0 = 1;
	TR0 = 1;
	P2 = 0xF7;
	P0 = LedChar[cnt];
	
	while(1)
	{
		if(KeySta != backup)
		{
			if(backup == 0)
			{
				cnt++;
				if(cnt >= 10)
				{
					cnt = 0;
				}
				P0 = LedChar[cnt];
			}
			backup = KeySta;
		}
	}
}

void InterruptTimer0() interrupt 1
{
	static unsigned char keybuf = 0xff;
	
	TH0 = 0Xf8;
	TL0 = 0xcd;
	keybuf = (keybuf<<1) | KEY4;
	if (keybuf == 0x00)
	{
		KeySta = 0;
	}
	else if(keybuf == 0xff)
	{
		KeySta = 1;
	}
	else
	{}
}