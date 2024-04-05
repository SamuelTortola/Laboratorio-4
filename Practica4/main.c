//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 4
// Hardware: Atmega238p
// Creado: 5/04/2024
//Última modificación: 05/04/2024
//******************************************************************************

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#define F_CPU 16000000

void setup(void);
void delay(uint8_t ciclos);
int conteo = 0;


int main(void)
{
    setup();
	
    while (1) 
    {
		
	}

}


void setup(void){
	DDRB = 0b00011100;   //PB0, PB1 como entrada. PB2, PB3, PB4 como salida
	PORTB = 0b00000011;		//pull up encendido en PB0 y PB1
	
	
	DDRD = 0b11111111;  //Salida hacia LEDs
	PORTD =0b00000000;	//Iniciamos los leds apagados
	
	PORTB |= (1<<PORTD2); //TRANSISTOR DE LEDS
	
	PCMSK0 |= (1<<0)|(1<<1); //PCINT0, PCINT1
	PCICR |= (1<<0); //Mascara de interrupción
	sei(); //Activar interrupciones
	
    
}

void delay(uint8_t ciclos){
	for(uint8_t j = 0; j < ciclos; j++){
			for(uint8_t i = 0; i < 255; i++){
				
			}
	}
}

ISR(PCINT0_vect){
	if(((PINB) & (1<<0)) == 0){
		conteo ++;
	}
	
	if(((PINB) & (1<<1)) == 0){
			conteo --;
		}
		
	PORTD = conteo;
	
}

