#include "reg51.h"                                                                                                                
#define KEYPORT = P1                                                                                                   

unsigned char i;                                                                                                                                         
unsigned char Keynum;                                                                                                        
unsigned char Line;                                       //行                                                                 
unsigned char Mask;                                                                                                         
unsigned char Col;                                        //列                                                                      
unsigned char ScanCode;                                                                                              

unsigned char code psegs_table[] =                                                                                            {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};                                                                //共阳极数码管0-F                                                                                                                                            

void delay_ms(unsigned int t)                      //ms延时程序                                                                      
{                                                                                                                                       
    unsigned char i;                                                                                                                 
     unsigned int x;                                                                                                                  
     x=t;                                                                                                                              
     while(x--)                                                                                                                        
     {                                                                                                                                          
             for(i=0;i<113;i++);                                                                                                              
     }                                                                                                                                           
}                                                                                                                                        
/* 键盘扫描程序 */                                                                                                                                        
void keyscan()                                                                                                           
{                                                                                                                                   
    while (1)              //循环扫描键盘       
    {                                                                                                                            
       P1 = 0xff;          //开始                                                         
        Line = 1;                                                                                         
        Mask = 0x01;                                                                                         

        for(i = 0; i < 4; i++)                                                                                   
        {                                                                                                                                                                                    
            P1 = ~Mask;                                                                                    

            ScanCode = P1 & 0xf0;                                                                                          

            if (ScanCode != 0xf0)                                                                                         
            {                                                                                                                                                                                    
                delay_ms(5);                                                                               
            }                                                                                                                                                                                    

            ScanCode = P1 & 0xf0;                                                                                          

            switch(ScanCode)                                                
            {                                                                                                                                                                                    
                case 0xe0:Col=1;break;                                                                                          
                case 0xd0:Col=2;break;                                                                   
                case 0xb0:Col=3;break;                                                                          
                case 0x70:Col=4;break;                                                                  
                default  :Col=0;break;                                                                                          
            }                                                                                                                                                                                    

            if (Col > 0)                                                                                                                                           
            {   
                //根据行列计算键值                                                                                                                                                         
                Keynum = (Line - 1) * 4 + Col;                                                                  
                //通过P0口接数码管显示                                                                                                          
                P0 = psegs_table[Keynum-1];                                                                                          

                while(1)                                                                                           
                {                                                                                
                    ScanCode = P1 & 0xf0;                                                                                
                    if(ScanCode == 0xf0)                                                               
                    {                                                                                                                                              
                        break;                                                    
                    }                                                                                                                       
                }                                                                                
                Mask <<= 1;                                                                                                      
                Line++;                                                                                                 
            }                                                                                                                        
        }                                                                                                                                      
    }  
}                                                                                                                                   

void main()                                                                                         
{                                                                                                                                     
    while(1)                                                                                                                      
    {                                                                                                                                 
        keyscan();                                                                                         
    }                                                                                                                                 
}                                                                      