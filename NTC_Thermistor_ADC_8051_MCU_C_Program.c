#include<reg51.h>
#include<math.h>
sbit LED_GREEN=P0^0;
sbit LED_YELLOW=P0^1;
sbit LED_RED=P0^2;
sbit wr=P3^6;
sbit rd=P3^5;
sbit intr=P3^7;
sbit rs=P3^2;
sbit rw=P3^3;
sbit en=P3^4;
const double Rb = 9710.0;
const double ADC_max = 255.0;
const double BETA = 3950.0;
const double Room_Temp = 298.15; // room temperature in Kelvin
const double RES_Room_Temp = 10000.0;
void ser(unsigned char data1);
void conv_ASCII(unsigned char value);
int adc();
double temperature(unsigned char val);
void delay(unsigned int v);
void main()
{
LED_GREEN=0;
LED_YELLOW=0;
LED_RED=0;
intr=1;
P1=0x0FF;
TMOD=0x20;
SCON=0x50;
TH1=0xFD;
TR1=1;
while(1)
{
unsigned char tCelsius;
 tCelsius=adc();
if(tCelsius<30)
{
LED_GREEN=1;
 LED_YELLOW=0;
 LED_RED=0;
}
else if(tCelsius>=30 && tCelsius<=90)
{
LED_YELLOW=1;
LED_GREEN=0;
 LED_RED=0;
}
else if(tCelsius>90)
{
LED_RED=1;
LED_GREEN=0;
 LED_YELLOW=0;
}
delay(150);
}
}
//ANALOG TO DIGITAL CONVERSION
int adc()
{
char adc_val,temp_cel;
rd=1;
wr=0;
delay(50);
wr=1;
while(intr==1);
rd=0;
adc_val=P1;
temp_cel=temperature(adc_val);
conv_ASCII(temp_cel);
return temp_cel;
}
double temperature(unsigned char val)
{
 // variables that live in this function
 double RNtc = 0; // Holds thermistor resistance value
 double tK = 0; // Holds calculated temperature
 double tC = 0; // Hold temperature in celsius
 RNtc = Rb * ( (ADC_max / val) - 1);
 tK = (BETA * Room_Temp) /
 (BETA + (Room_Temp * log(RNtc / RES_Room_Temp)));
 tC = tK - 273.15; // convert kelvin to celsius
 return tC; // Return the temperature in Celsius
}
//CONVERT TO ASCII
void conv_ASCII(unsigned char value)
{
 unsigned char x1,x2,x3;
 x1=(value/10);
 x1=x1+(0x30);
 x2=value%10;
 x2=x2+(0x30);
 x3=0xDF;
 ser(x1);
 ser(x2);
}
//SERIAL COMMUNICATION
void ser(unsigned char data1)
{
SBUF=data1;
while(TI==0);
TI=0;
}
void delay(unsigned int v)
{
unsigned char i;
for(i=0;i<v;i++);
}