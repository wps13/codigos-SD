/* Questão 4 
 *
 * Implementar um somatório de wi*xi, 0<=i<=4
 * xi é enetrada analógica
 * wi é o ganho, que é um valor aleatório entre 0 e 1
 * usando um MCU, que no nosso caso será um atmega328p
 */

#include <avr/io.h> //biblioteca para os adcs
#include <stdlib.h> 

float x[5]; //variaveis para leituras dos sensores
float w[5]; //ganhos
float y;   //valor final do pwm

void setup(){
	ADMUX = 0b01000000; //seleciona avcc como referência
	ADCSRA = 0b10000111; //habilita ad e prescaler =128
	TCCR0A = 0b10100011; //Clear OCRA na comparação,usa fast pwm
	TCCR0B = 0b00001011; // Define OCRA como valor para comparção e prescaler = 64
	OCRA = 0; //registrador que gera pwm
}

void leituraAio(){
	ADMUX &= 0b01000000; //Coloca no ADC0
	ADCRSA |= 0b01000000; //Inicia Conversão
	while(!(ADCRSA & 0b00010000; //Espera conversão finalizar
	x[0] = ADC;

	ADMUX |= 0b01000001; //Coloca no ADC1
	ADCRSA |= 0b01000000; //Inicia Conversão
	while(!(ADCRSA & 0b00010000; //Espera conversão finalizar
	x[1] = ADC;

	ADMUX &= 0b01000000;//zerar admux 
	ADMUX |= 0b01000010; //Coloca no ADC2
	ADCRSA |= 0b01000000; //Inicia Conversão
	while(!(ADCRSA & 0b00010000; //Espera conversão finalizar
	x[2] = ADC;

	ADMUX |= 0b01000011; //Coloca no ADC3
	ADCRSA |= 0b01000000; //Inicia Conversão
	while(!(ADCRSA & 0b00010000; //Espera conversão finalizar
	x[3] = ADC;

	ADMUX &= 0b01000000;//zerar admux 
	ADMUX |= 0b01000100; //Coloca no ADC4
	ADCRSA |= 0b01000000; //Inicia Conversão
	while(!(ADCRSA & 0b00010000; //Espera conversão finalizar
	x[4] = ADC;
}
void ganhos(){
	srand((unsigned) time(&t));

	for(int i=0;i<5;i++){
		w[i] = (float)rand()/(float)(RAND_MAX/1.0);
	}	
}
void main(){
	for(int i=0;i<5;i++){
		y += w[i]*x[i]; 
	}
	OCRA = y; // gera pwm 	
}
