#include <stdio.h>
#include <stdint.h>
#include "ILI9341.h"

//orden puerto D con pines de pantalla
//	RESET/SIGNAL		CHIPSELEC		COM/DATA			WRITESIGNAL			READSIGNAL
//	RST							CS					SR						WR							DR

#define RST 	0x10
#define CS 		0x08
#define SR 		0x04
#define WR 		0x02
#define RD 		0x01

volatile uint16_t LCD_W=ILI9341_TFTWIDTH;
volatile uint16_t LCD_H=ILI9341_TFTHEIGHT;

void DATA(uint8_t DATO){
GPIO_PORTD_DATA_R |= SR;
	GPIO_PORTD_DATA_R &= ~CS;
	GPIO_PORTA_DATA_R = DATO;
	GPIO_PORTD_DATA_R |= CS;
}
void CMD(uint8_t COMANDO){
GPIO_PORTD_DATA_R &= ~SR;
	GPIO_PORTD_DATA_R &= ~CS;
	GPIO_PORTA_DATA_R = COMANDO;
	GPIO_PORTD_DATA_R |= CS;
}
//EN ORDEN CON PUERTO A
void init_portA(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R0; //HABILITA EL PUERTO p
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0)==0){};//VERIFICA QUE ESTE ENCENDIDO EL PUERTO 
	GPIO_PORTA_DIR_R |=0xFF;//ELIJE DE FORMA DIGITAL EL PIN
	GPIO_PORTA_AFSEL_R |=0x00;//
	GPIO_PORTA_DEN_R |=0xFF;	
	GPIO_PORTA_AMSEL_R=0x00;
}
//orden puerto D con pines de pantalla
//	RESET/SIGNAL		CHIPSELEC		COM/DATA			WRITESIGNAL			READSIGNAL
//	RST							CS					RS						WR							RD
//	D4							D3					D2						D1							D0
void init_portD(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R3; //HABILITA EL PUERTO p
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3)==0){};//VERIFICA QUE ESTE ENCENDIDO EL PUERTO 
	GPIO_PORTD_DIR_R |=0x1F;//ELIJE DE FORMA DIGITAL EL PIN
	GPIO_PORTD_AFSEL_R |=0x00;//
	GPIO_PORTD_DEN_R |=0x1F;	
	GPIO_PORTD_AMSEL_R=0x00;
}

void Delay(uint32_t tiempo){
	NVIC_ST_CTRL_R = 0x04; //Configuracion para que la frecuencia sea 16 Mhz
	NVIC_ST_CURRENT_R = 0x000000; //Registra y escribe un valor
	NVIC_ST_RELOAD_R = tiempo; // Cargando el tiempo
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; //Preguntando la bandera
	while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; //Desactivando la bandera
}

void Inicializar_Pantalla(void){
	GPIO_PORTD_DATA_R |= RST;
	// delay 1ms
	GPIO_PORTD_DATA_R &= ~RST;
	// delay 10ms
	GPIO_PORTA_DATA_R |= RST;
	// delay 120ms
	
	CMD(ILI9341_SOFTRESET);
	Delay(5000*16);
	CMD(ILI9341_DISPLAYOFF);
	
	CMD(ILI9341_POWERCONTROL1);
	DATA(0x26);	
	
	CMD(ILI9341_POWERCONTROL2);
	DATA(0x11);	
	
	CMD(ILI9341_VCOMCONTROL1);
	DATA(0x5c);	
	DATA(0x4c);	
	
	CMD(ILI9341_VCOMCONTROL2);
	DATA(0x94);
	
	CMD(ILI9341_MEMCONTROL);
	DATA(0x48);	
	
	CMD(ILI9341_PIXELFORMAT);
	DATA(0x55);	
	
	CMD(ILI9341_FRAMECONTROL);
	DATA(0x00);	
	DATA(0x1B);	
	
	CMD(ILI9341_DISPLAYFUNC);
	DATA(0x08);	
	DATA(0x82);	
	DATA(0x27);	
	
	CMD(ILI9341_DigGammaControl);
	DATA(0x00);
	
	CMD(ILI9341_GAMMASET);
	DATA(0x01);
	
	CMD(ILI9341_PGammaCorrection);
	DATA(0x0F);
	DATA(0x31);
	DATA(0x2B);
	DATA(0x0C);
	DATA(0x0E);
	DATA(0x08);
	DATA(0x4E);
	DATA(0xF1);
	DATA(0x37);
	DATA(0x07);
	DATA(0x10);
	DATA(0x03);
	DATA(0x0E);
	DATA(0x09);
	DATA(0x00);
	
	CMD(ILI9341_NGammaCorrection);
	DATA(0x00);
	DATA(0x0E);
	DATA(0x14);
	DATA(0x03);
	DATA(0x11);
	DATA(0x07);
	DATA(0x31);
	DATA(0xC1);
	DATA(0x48);
	DATA(0x08);
	DATA(0x0F);
	DATA(0x0C);
	DATA(0x31);
	DATA(0x36);
	DATA(0x0F);
	
	CMD(ILI9341_SLEEPOUT);
	// delay 120ms
	
	CMD(ILI9341_DISPLAYON);
	CMD(ILI9341_MEMORYWRITE);
//	fill_init();
}

void setPage(uint16_t StartPage,uint16_t EndPage){
    CMD(ILI9341_PAGEADDRSET);                                                      /* Column Command address       */
    DATA(StartPage);
    DATA(EndPage);
}
void setCol(uint16_t StartCol,uint16_t EndCol)
{
    CMD(ILI9341_COLADDRSET);                                                      /* Column Command address       */
    DATA(StartCol);
    DATA(EndCol);
}
void setXY(uint16_t poX, uint16_t poY)
{
    setCol(poX, poX);
    setPage(poY, poY);
    CMD(ILI9341_MEMORYWRITE);
}
//void setPixel(uint16_t poX, uint16_t poY,uint16_t color)
//{
//    setXY(poX, poY);
//    DATA(color);
//}
//void setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
//{
//	CMD(ILI9341_COLADDRSET);
//	DATA(x1>>8);
//	DATA(x1);
//	DATA(x2>>8);
//	DATA(x2);

//	CMD(ILI9341_PAGEADDRSET);
//	DATA(y1>>8);
//	DATA(y1);
//	DATA(y2);
//	DATA(y2);

//	CMD(ILI9341_MEMORYWRITE);//meory write
//}
//void pushcolour(uint16_t colour)
//{
//	DATA(colour>>8);
//	DATA(colour);
//}
//void ili9341_clear(uint16_t colour)
//{
//	uint16_t i,j;
//	setaddress(0,0,LCD_W-1,LCD_H-1);

//	for(i=0;i<LCD_W;i++)
//	{
//		for(j=0;j<LCD_H;j++)
//		{
//			pushcolour(colour);
//		}
//	}
//}
//void drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1) //pixels will always be counted from right side.x is representing LCD width which will always be less tha 240.Y is representing LCD height which will always be less than 320
//{
//	if ((x3 < 0) ||(x3 >= LCD_W) || (y3 < 0) || (y3 >= LCD_H))
//	{
//		return;
//	}

//	setaddress(x3,y3,x3+1,y3+1);
//	pushcolour(colour1);
//}