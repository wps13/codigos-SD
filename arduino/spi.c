#include "avr/io.h"

uint8_t recebido = 0;
uint8_t enviar = 0;

void setup(){
	//Habilita PB4(D12) como saída, uma vez que esse pino
	// é usado como MISO(Master In Slave Out),
	//o que significa que é usado para enviar dados 
	//do escravo para o mestre
	//Já o PB3(D11) é usado para receber dados(MOSI)
	DDRB = 0b0001000;
	//Registrador usado para controlar o spi
	//Habilita SPI, define a taxa de clock = fck/16 e
	//define arduino como escravo
	SPCR = 0b01000001;
	
}

void main(){
	while(1){
		//define o dado a ser enviado no registrador de dados
		SPDR = enviar;
		//espera finalizar transmissão do dado
		while(!(SPSR & (1<<SPIF)));
		//armazena valor recebido
		recebido = SPDR;
	}
}
