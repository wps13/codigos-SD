#include "avr/io.h"

uint8_t recebido = 0;
uint8_t enviar= 0;

uint8_t ldrConversor();

void setup(){
	//PB4(D12) = MISO, envia dados
	//PB3(D11) = MOSI, recebe dados
	DDRB = 0b00010000;
	SPCR = 0b01000001;
	ADMUX = 0b01000000;  //seleciona AVCC como referencia
    ADCSRA = 0b10000111; //habilita adc e fator de divisao da f = 128
}

void main(){
	while(1){
		enviar = ldrConversor();
		//seleciona dado a ser enviado
		SPDR = enviar;
		//Aguarda fim do envio
		while(!(SPSR & (1<<SPIF)));
		recebido = SPDR;
	}
}

uint8_t ldrConversor(){
	//seleciona a ADC0
	ADMUX &= 0b01000000;
	//Inicia conversao
	ADCSRA |= 0b01000000;
	while(!(ADCSRA & 0b00010000));
	return ADC;
}


