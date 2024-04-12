//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 4
// Hardware: Atmega238p
// Creado: 5/04/2024
//Última modificación: 11/04/2024
//******************************************************************************


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void setup(void);
void initADC(void);

int conteo = 0; //Variable para contador de 0 a 255
uint8_t dato1 = 0;
uint8_t dato2 = 0;
uint8_t datos1[4], datos2[4];
int activa = 1, activa1= 1, activa2 = 1;
int dato = 0, dato3 = 0, dato4 = 0, dato5 = 0, dato6 = 0, dato7 = 0, i = 0, n = 0, bin[8];
const uint8_t  lista[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	
int ADCMAYOR = 0, ADCMENOR = 0;


void setup(void){
	DDRB = 0b00111100;   //PB0, PB1 como entrada. PB2, PB3, PB4, PB5 como salida
	PORTB = 0b00000011;		//pull up encendido en PB0 y PB1
	
	DDRD = 0b11111111;  //Salida hacia LEDs
	PORTD = 0b00000000;	//Iniciamos los leds apagados
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	PCMSK0 |= (1<<0)|(1<<1); //PCINT0, PCINT1
	PCICR |= (1<<0); //Mascara de interrupción
	initADC();
	
	sei(); //Activar interrupciones
}

void initADC(){
	ADMUX = 0;
	ADMUX |= (1<<REFS0);  //Referencia AVCC = 5V
	ADMUX &= ~(1<<REFS1); //Justificación a la izquierda
	ADMUX |= (1<<ADLAR); 
	ADCSRA = 0;
	ADCSRA |= (1<<ADIE);  //Habilitando interrupcion de ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  //Habilitando prescaler de 16M/128 frecuencia = 125Khz
	ADCSRA |= (1<<ADEN);   //Habilitando el ADC
}



int main(void)
{
    setup();
	
    while (1) 
    {
		
		PORTB ^= (1<<2);   //ENCENDER LEDS
        PORTD=conteo;  //Mostrar el conteo de los LEDs 
		
		_delay_ms(6);
		PORTB ^= (1<<2) | (1<<3);  //ENCENDER DISPLAY 1 Y APAGAR LEDS
		PORTD = lista[ADCMAYOR];   //Mostrar el dato del display 1
		
		_delay_ms(6);
		PORTB ^=  (1<<3)| (1<<4);   //ENCENDER DISPLAY 2 Y APAGAR DISPLAY 1
		PORTD = lista[ADCMENOR];   //Mostrar el dato del display 2
		
		_delay_ms(6);
		PORTB ^=  (1<<4);   //APAGAR DISPLAY 2
		
		ADCSRA |=(1<<ADSC);  //Leer ADC0
		
		if(dato3 > 1){   //Cuando se mueva el potenciometro
			if(dato3 > conteo){  //Verificar si mayor el ADC
			PORTB |= (1<<5);  //Encender LED de alarma
			activa1 = 2;  //activa el else
			}
			else{
				if(activa1 == 2){   //Si conteo es menor
					PORTB ^= (1<<5);  //Apaga el LED
					activa1 = 1;  //Desactiva el else
				}
				PORTB |= (0<<5);  //Mantiene apagado el LED
			}
		}
		else{      //Se repite el else cuando esta fuera de la función
			if(activa1 == 2){
				PORTB ^= (1<<5);
				activa1 = 1;
			}
			PORTB |= (0<<5);
		}
	}

}


ISR(PCINT0_vect){
	
	
	if(((PINB) & (1<<0)) == 0){   //Condicional que compara si se presionó el pulsador 1
		conteo ++;           //Aumenta el valor del contador 
		if(conteo >= 255){   //Bloqueo de conteo a 255
			conteo = 255;
		}
		
	}
	
	if(((PINB) & (1<<1)) == 0){   //Condicional que compara si se presionó el pulsador 2
			conteo --;     // Resta el valor del contador
				if(conteo <= 0){  //Bloqueo de conteo a 0
					conteo = 0;
				}
		}
}


ISR(ADC_vect){
	dato3 = ADCH;   //Contador general
	
		uint16_t adc_val16 = ADC;   //Se leer el valor del ADC
		uint8_t adcValue = (adc_val16 >> 8);  //Corrimiento
		
		ADCMAYOR = adcValue >> 4;   //Se separan los valores de 0 a 255 de 8 bits, a dos arreglos de 4 bits
		ADCMENOR = adcValue & 0x0F;
	  ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción

}
