#include <stdint.h> 
#include "tm4c1294ncpdt.h"

#define RST 	0x10
#define CS 		0x08
#define RS 		0x04
#define WR 		0x02
#define RD 		0x01



void init_portA(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R0; //HABILITA EL PUERTO A
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0)==0){};//VERIFICA QUE ESTE ENCENDIDO EL PUERTO 
	GPIO_PORTA_DIR_R |=0xFF;//SE ACTIVAN TODOS LOS PINES COMO SALIDA
	GPIO_PORTA_AFSEL_R |=0x00;//
	GPIO_PORTA_DEN_R |=0xFF;	
	GPIO_PORTA_AMSEL_R=0x00;
}

void init_portD(void){
	SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R3; //HABILITA EL PUERTO p
	while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3)==0){};//VERIFICA QUE ESTE ENCENDIDO EL PUERTO 
	GPIO_PORTD_DIR_R |=0x1F;//ELIJE DE FORMA DIGITAL EL PIN
	GPIO_PORTD_AFSEL_R |=0x00;//
	GPIO_PORTD_DEN_R |=0x1F;	
	GPIO_PORTD_AMSEL_R=0x00;
}

void write_parameter(uint8_t param){
	GPIO_PORTA_DATA_R |= RS;
	GPIO_PORTA_DATA_R &= ~CS;
	GPIO_PORTD_DATA_R = param;
	GPIO_PORTA_DATA_R |= CS;
}

void send_cmd(uint8_t cmd){
	GPIO_PORTA_DATA_R &= ~RS;
	GPIO_PORTA_DATA_R &= ~CS;
	GPIO_PORTD_DATA_R = cmd;
	GPIO_PORTA_DATA_R |= CS;
}

void init(void){
	init_portD();
	init_portA();
	
	GPIO_PORTA_DATA_R |= RST;
	delay(1000);
	GPIO_PORTA_DATA_R &= ~RST;
	delay(10000);
	GPIO_PORTA_DATA_R |= RST;
	delay(120000);
	
	send_cmd(0x01);
	delay(50*16);
	send_cmd(0x28);
	
	send_cmd(0xc0);
	write_parameter(0x26);	
	
	send_cmd(0xC1);
	write_parameter(0x11);	
	
	send_cmd(0xC5);
	write_parameter(0x5c);	
	write_parameter(0x4c);	
	
	send_cmd(0xC7);
	write_parameter(0x94);
	
	send_cmd(0x36);
	write_parameter(0x48);	
	
	send_cmd(0x3A);
	write_parameter(0x55);	
	
	send_cmd(0xB1);
	write_parameter(0x00);	
	write_parameter(0x1B);	
	
	send_cmd(0xB6);
	write_parameter(0x08);	
	write_parameter(0x82);	
	write_parameter(0x27);	
	
	send_cmd(0xF2);
	write_parameter(0x00);
	
	send_cmd(0x26);
	write_parameter(0x01);
	
	send_cmd(0xE0);
	write_parameter(0x0F);
	write_parameter(0x31);
	write_parameter(0x2B);
	write_parameter(0x0C);
	write_parameter(0x0E);
	write_parameter(0x08);
	write_parameter(0x4E);
	write_parameter(0xF1);
	write_parameter(0x37);
	write_parameter(0x07);
	write_parameter(0x10);
	write_parameter(0x03);
	write_parameter(0x0E);
	write_parameter(0x09);
	write_parameter(0x00);
	
	send_cmd(0xE1);
	write_parameter(0x00);
	write_parameter(0x0E);
	write_parameter(0x14);
	write_parameter(0x03);
	write_parameter(0x11);
	write_parameter(0x07);
	write_parameter(0x31);
	write_parameter(0xC1);
	write_parameter(0x48);
	write_parameter(0x08);
	write_parameter(0x0F);
	write_parameter(0x0C);
	write_parameter(0x31);
	write_parameter(0x36);
	write_parameter(0x0F);
	
	send_cmd(0x11);
	delay(120000);
	
	//send_cmd(0x21);
	
	send_cmd(0x29);
}

