#include <avr/io.h>
#include <avr/interrupt.h>

#define fosc 16000000 //frequencia do atmega328p (16MHz)
#define BAUD 9600     //taxa de transmissão
#define myubrr fosc/16/BAUD-1

uint8_t dado = 6;
uint8_t recebido =0;

//A comunicação por usart usa os pinos D0(RX) e D1(TX)

//Configurações dos registradores para serial
void setup(){
	//define a taxa de transmissão
	UBRR0H =(unsigned char)(myubrr>>8);
	UBRR0L =(unsigned char)myubrr;
	UCSR0B = (1<<RXENO)|(1<<TXENO);//Habilita transmissão
	UCSR0C=(1<<USBS0)|(3<<UCSZ00);
}

void main(){
	while(1){
		 //Aguarda finalizar o esvaziamento do buffer
	 	while(!(UCSR0A & (1<<UDRE));
		 //define dado a ser enviado
	 	UDR0 = dado;
		while(!(UCSR0A & (1<<RXC)));
		recebido = UDR0;
	}

}
 
