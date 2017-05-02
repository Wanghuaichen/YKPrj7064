#include <mega16.h>
#include <delay.h>
#include <stdio.h>
/*#asm
   .equ __lcd_port=0x1B ;PORTA
#endasm
#include <lcd.h>   
*/ 
#define Test_LED  PORTA.3 
#define FIRE_in   PORTD.3
#define RSTN      PORTD.4
#define START     PORTD.5
#define STOP      PORTD.6

#define ss        PORTB.4

#define IO1       PORTB.2
#define IO2       PORTC.0
#define IO3       PORTC.1
#define IO4       PORTD.7
#define IO5       PORTB.0
 
void TDC_init(void);
void measurment(void);
float read_TDC_Time(unsigned long int data);

bit flag=0,SPI_flag=0;
unsigned long int rdata;
unsigned char data3,data2,data1,data0;
unsigned char lcd[16];
volatile unsigned int n=1; //min=1,max=351
 
              
//-----------------------------------------------------------
//-----------------------------------------------------------
interrupt [2] void EXT_INT0_ISR(void)//ALU ready
{ 
Test_LED=1;
 ss=0;
 
 data3=SPDR;
 while(SPI_flag == 0);
 SPI_flag=0;
 data2=SPDR; 
 while(SPI_flag == 0);
 SPI_flag=0;
 data1=SPDR;
 while(SPI_flag == 0);
 SPI_flag=0;
 data0=SPDR;
 while(SPI_flag == 0);
 SPI_flag=0; 
 
 ss=1;
 flag=1;    
}
//-----------------------------------------------------------
//-----------------------------------------------------------
interrupt [7] void Timer1_comA_ISR(void)//260us
{  
  //over range 
 //lcd_gotoxy(13,1);
 //lcd_putsf("OVR");
}
//-----------------------------------------------------------
//-----------------------------------------------------------
interrupt[11] void SPI_ISR(void)
{
 SPI_flag=1;
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void main(void)
{
 DDRA.3=1;
 Test_LED=0;

 DDRB=0xB0;
 PORTB=0x1F;

 PORTD.2=1;//pullup
 
 DDRD.3=1; 
 FIRE_in=0; //not used
 
 DDRD.4=1;
 RSTN=1;  
 
 DDRD.5=1;
 START=0; 
 
 DDRD.6=1;
 STOP=0;
 
 PORTC.6=1;
 PORTC.7=1;   
 
// SPI initialization
// SPI Type: Master
// SPI Mode: 1     
// SPI Clock Rate: 1.8432MHz
// SPI Data Order: MSB First
 SPCR=0xD5;
 SPSR=0x01;
 
 #asm
    in   r30,spsr
    in   r30,spdr
 #endasm
                     
 GICR=0x40;
 MCUCR=0x02;
 MCUCSR=0;
 
 TCCR1A=0;
 TCCR1B=0;
 TCNT1=0;
 //OCR1A=3833;  //260us
 OCR1A=4000; 
 
 TIMSK=0x10;

 //lcd_init(16);

 ACSR=0x80;
 SFIOR=0x00;
 
#asm("sei")  
 
 //lcd_putsf("ready");
 //delay_ms(1000);
 //lcd_clear();
 //TDC_init();

while(1)
{
 ss=0;       
 SPDR=0xB4;
 while(SPI_flag == 0);
 SPI_flag=0;
 while(SPI_flag == 0);
 SPI_flag=0;
 rdata=SPDR; 
 ss=1; 
} 

 while(1)
  {    
    if(flag == 1)
    {
     flag=0;
     rdata =  (long)data3<<24;
     rdata |= (long)data2 <<16;
     rdata |= (long)data1 << 8;
     rdata |= data0;
     
     //sprintf(lcd,"Tt=%.4fns",read_TDC_Time(rdata));
     //lcd_gotoxy(0,0);
     //lcd_puts(lcd);
    }  
  }
} 
//-----------------------------------------------------------
//-----------------------------------------------------------
void TDC_init(void)
{                  
//------------------TDC_RST
 RSTN=0;
 
 #asm("nop")  //=67.82ns
              //>50ns
 RSTN=1; 
 
 #asm
 nop
 nop
 nop
 nop
 #endasm      //>200ns 
 ss=0;  
//---------------------------
 SPDR=0x50;//POR   
 while(SPI_flag == 0);
 SPI_flag=0;  
//---------------------------  
 //write to reg0
 SPDR=0x80;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0xC6;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x68;
 while(SPI_flag == 0);
 SPI_flag=0;
//---------------------------  
 //write to reg1
 SPDR=0x81;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x21;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x42;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0);  
 SPI_flag=0;
//---------------------------  
 //write to reg2
 SPDR=0x82;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x20;
 while(SPI_flag == 0);   
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0); 
 SPI_flag=0;
//---------------------------  
 //write to reg3
 SPDR=0x83;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x28;
 while(SPI_flag == 0); 
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0); 
 SPI_flag=0; 
 SPDR=0;
 while(SPI_flag == 0); 
 SPI_flag=0;
//---------------------------  
 //write to reg4
 SPDR=0x84;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x20;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0;
//---------------------------  
 //write to reg5
 SPDR=0x85;
 while(SPI_flag == 0);
 SPI_flag=0;
 SPDR=0x10;
 while(SPI_flag == 0);
 SPI_flag=0;  
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0; 
 SPDR=0;
 while(SPI_flag == 0);
 SPI_flag=0;
//---------------------------  
 SPDR=0x70;//init 
 while(SPI_flag == 0);
 SPI_flag=0;
 
 ss=1; 

 measurment();      
//---------------------------    
}
//-----------------------------------------------------------
//-----------------------------------------------------------  
void measurment(void)
{
 char lcd_array[15]; 
 unsigned int i=0;
 float Time=0;
  
 TCNT1=0;
 TCCR1B=0x09;
 
 START=1;
 START=0;

 for(i=0;i<n;i++);//6+7*n=cycle  //Time=cycle*67.817
           
 STOP=1;
 
 Time=TCNT1;
 ss=0;
 SPDR=0xB0;
 while(SPI_flag == 0);
 SPI_flag=0;
 ss=1;
 delay_us(5);//wait for 4.6us 

 STOP=0;
 
 TCCR1B=0;
 
 Time /= 14.7456;
 Time -= 0.406901062;
   
 //sprintf(lcd_array,"Tu=%.4fus",Time);
 //lcd_gotoxy(0,1);
 //lcd_puts(lcd_array);
}
//-----------------------------------------------------------
//----------------------------------------------------------- 
float read_TDC_Time(unsigned long int data)
{ 
unsigned char array=0,index_array[16];
unsigned int DataH,DataL;
float T;

 DataH=(int)((data & 0xFFFF0000) >> 16);
 DataL=(int)(data & 0x0000FFFF);

 for(array=0;array<16;array++)
 {
  if((DataH & (0x8000 >> array)) != 0) index_array[array]=1;
  else index_array[array]=0;
  T += ((int)index_array[array] * (0x8000 >> array));
 }
 
if((DataL & 0x8000) != 0) T += 0.5;
if((DataL & 0x4000) != 0) T += 0.25;
if((DataL & 0x2000) != 0) T += 0.125;
if((DataL & 0x1000) != 0) T += 0.0625;
if((DataL & 0x0800) != 0) T += 0.03125;
if((DataL & 0x0400) != 0) T += 0.015625;
if((DataL & 0x0200) != 0) T += 0.0078125;
if((DataL & 0x0100) != 0) T += 0.00390625;
if((DataL & 0x0080) != 0) T += 0.001953125;
if((DataL & 0x0040) != 0) T += 0.0009765625;
if((DataL & 0x0020) != 0) T += 0.00048828125;
if((DataL & 0x0010) != 0) T += 0.000244140625;
if((DataL & 0x0008) != 0) T += 0.0001220703125;
if((DataL & 0x0004) != 0) T += 0.00006103515625;
if((DataL & 0x0002) != 0) T += 0.000030517578125;
if((DataL & 0x0001) != 0) T += 0.0000152587890625;

//-----------
T *= 65;
T /= 1000;
//-----------
 
//T *= 0.00974325469;

return T;
}
//-----------------------------------------------------------
//-----------------------------------------------------------