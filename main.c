#include <stdint.h> 
#include "tm4c1294ncpdt.h"

#define RST 	0x10 //RS
#define CS 		0x08 //
#define RS 		0x04 //D/CX
#define WR 		0x02 //WR
#define RD 		0x01 //RDX



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
void delay(uint32_t tiempo){
	NVIC_ST_CTRL_R = 0x04; //Configuracion para que la frecuencia sea 16 Mhz
	NVIC_ST_CURRENT_R = 0x000000; //Registra y escribe un valor
	NVIC_ST_RELOAD_R = tiempo; // Cargando el tiempo
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; //Preguntando la bandera
	while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; //Desactivando la bandera
}

void write_parameter(uint8_t param){
	GPIO_PORTD_DATA_R |= RS;
	GPIO_PORTD_DATA_R |= RD;
	GPIO_PORTD_DATA_R &= ~CS;
	GPIO_PORTA_DATA_R = param;
	GPIO_PORTD_DATA_R |= CS;
}

void send_cmd(uint8_t cmd){
	GPIO_PORTD_DATA_R &= ~RS;
	GPIO_PORTD_DATA_R |= RD;
	GPIO_PORTD_DATA_R &= ~CS;
	GPIO_PORTA_DATA_R = cmd;
	GPIO_PORTD_DATA_R |= CS;
}

void init(void){
	init_portD();
	init_portA();
	
	//Se genera un flanco de subida

	GPIO_PORTD_DATA_R = RST; //Se pone en uno 
		delay(16000000/190);
	GPIO_PORTD_DATA_R =~RST; //Se pone en cero 
		delay(16000000/20);
		GPIO_PORTD_DATA_R =RST; //Se pone en uno 
		delay(16000000/20);
	
	GPIO_PORTD_DATA_R |= CS;
	GPIO_PORTD_DATA_R |= RS;
	GPIO_PORTD_DATA_R |= RD;
	send_cmd(0x01); //Se envia el comando de Software Reset
	delay(16000000/190); //Se espera un tiempo 
	send_cmd(0x10);//modo sleep
	
	send_cmd(0xc0); //Se realiza un Power Control 1
	write_parameter(0x26);//Se configura a 4.75  volts 
	
	send_cmd(0xC1); //Power Control 2
	write_parameter(0x11); 	
	
	send_cmd(0xC5); //VCOM Control 1
	write_parameter(0x5c);	//Maximo voltaje 5V
	write_parameter(0x4c);	//Minimo voltaje -.6 volts
	
	send_cmd(0xC7); //VCOM Control 2
	write_parameter(0xC0);
	
	send_cmd(0x36); //Memory Access Control
	write_parameter(0x48);	//Se define como se enviar�n los datos 
	
	send_cmd(0xB6);//Display Function Control
	write_parameter(0x08);	//Establece modo escanaeo en el modo de no visualizaci�n 
	write_parameter(0x82);	//Se configura los modos en los que se recibira el dato y el despliegue
	write_parameter(0x27);	//Selecciona la direcci�n de desplazamiento de las salidas desde el controlador de origen.
	
	send_cmd(0x3A);// Pixel Format Set
	write_parameter(0x55);	//Se define el formato de 16 bits 
	
	
	send_cmd(0xB1);//Frame Rate Control
	write_parameter(0x00);	//Toma el fosc para la division
	write_parameter(0x1B);	//Toma 70 Hz de frame rate
	
	send_cmd(0xB6);//Display Function Control
	write_parameter(0x08);	//Establece modo escanaeo en el modo de no visualizaci�n 
	write_parameter(0x82);	//Se configura los modos en los que se recibira el dato y el despliegue
	write_parameter(0x27);	//Selecciona la direcci�n de desplazamiento de las salidas desde el controlador de origen.
	
	send_cmd(0xE2);//Digital Gamma Control 1
	write_parameter(0x00);//
	
	send_cmd(0x26);//Gamma Set
	write_parameter(0x01);
	
	send_cmd(0xE0);// Positive Gamma Correction
	//Es necesario enviar 15 parametros una vez ingresado este comando 
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
	
	send_cmd(0xE1);//Negative Gamma Correction 
	//Es necesario enviar la configuracion de este comando en 15 parametros
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
	
	send_cmd(0x11);//Exit sleep
	delay(16000000/20);
	send_cmd(0x29); //Display ON
	//send_cmd(0x21);
	

}



void Direccion_de_la_Pagina(uint16_t Inicio, uint16_t  Final){
	send_cmd(0x2B);
	write_parameter(0x00);
	write_parameter(Inicio);
	write_parameter(0x00);
	write_parameter(Final);	
}

void Columna_del_Cuadro(uint16_t Inicio, uint16_t  Final){
	send_cmd(0x2A);
	write_parameter(0x00);
	write_parameter(Inicio);
	write_parameter(0x00);
	write_parameter(Final);	
}

void Fondo_de_Pantalla(void){
Columna_del_Cuadro(0,239);													// Limitamos el Cuadro en columnas
Direccion_de_la_Pagina(0,319);										// Limitamos el pagina
send_cmd(0x2C);													// Habilitamos escribir en memoria. Realiza la transferencia de datos de MCU a la trama de memoria.
	

	for(int i=0; i<38400; i++){												// Realizamos recorrido de toda la pantalla
	  write_parameter(0xFF);												// Determinamos color de 1/4 de pantalla
		write_parameter(0xFF);												// Determinamos color de 1/4 de pantalla
		write_parameter(0xFF);												// Determinamos color de 1/4 de pantalla
		write_parameter(0xFF);												// Determinamos color de 1/4 de pantalla
	}	
 
}

int main(void){
	init();
	//Fondo_de_Pantalla();
}
