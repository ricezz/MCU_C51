#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define KeyPort P3
sbit dula=P2^1;
sbit wela=P2^0;
unsigned char code DuanMa[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

void DelayUs2x(unsigned char t);//us级延时函数声明 
void DelayMs(unsigned char t); //ms级延时

unsigned char KeyScan(void);//键盘扫描
unsigned char KeyPro(void);
void display(unsigned char aa);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num;   
	dula=1;
	P1=0x00;
	dula=0;
	wela=1;
	P1=0x00;
	wela=0;       
while (1)         //主循环
	{
		num=KeyPro();
	if(num!=0xff)
   {
		display(num);
	}
}
}
void display(unsigned char aa)
{
	dula=1;
	P1=DuanMa[aa];
	dula=0;		
}
/*------------------------------------------------
          按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
	unsigned char cord_h,cord_l;//行列值中间变量
	KeyPort=0x0f;            //列线输出全为0
	cord_h=KeyPort&0x0f;     //读入行线值
	if(cord_h!=0x0f)    //先检测有无按键按下
	{
		DelayMs(10);        //去抖
		if((KeyPort&0x0f)!=0x0f)
	{
    cord_h=KeyPort&0x0f;  //读入行线值
    KeyPort=cord_h|0xf0;  //输出当前列线值
    cord_l=KeyPort&0xf0;  //读入列线值

    while((KeyPort&0xf0)!=0xf0);//等待松开并输出
    return(cord_h+cord_l);//键盘最后组合码值
   }
  }return(0xff);     //返回该值
}