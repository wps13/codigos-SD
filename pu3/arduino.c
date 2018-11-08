#include "avr/io.h"

uint8_t sensortemp;
uint8_t sensorLuz;

uint8_t ldrConversor();
uint8_t temperaturaConversor();

void setup(){
	DDRB = 0b00010000; //PB4
	SPCR = 0b01000001; //habilita spi
    PORTB = 0b00000000;  //coloca as portas em 'low'
    ADMUX = 0b01000000;  //seleciona AVCC como referencia
    ADCSRA = 0b10000111; //habilita adc e fator de divisao da f = 128
}

void loop(){
	while(1){

    	sensortemp = temperaturaConversor();
    	sensorLuz = ldrConversor();
		SPDR = sensortemp;
		while(!(SPSR & (1<<SPIF)));
		SPDR = sensorLuz;
		while(!(SPSR & (1<<SPIF)));
	}
}

uint8_t ldrConversor()
{
    /* O valorLDR eh dependente da tensao que fica sobre o tensor, 
     * após a divisão de tensao, onde aqui seria com resistores de 10k
     *  Com o valor da tensao obtido, usaria a fórmula da resolucao
     * Tensão de entrada = 3
     * valorLDR = (3*1024)/5
     */
   
    /* Define a porta ADC0 como a que sera usada para conversao
     * Usa-se o operador '&' devido a necessidade de uso de bits 0
     * Caso esteja com o bit '1' em alguma porta, isso zera todas
     * possibilitando assim que a porta definida seja usada,
     * Uma vez que o codigo para o ADC0 é 0000.
     * Eh necessario tambem que seja definida a referencia, bit 6,
     * Pois, caso nao seja definida, a entrada nao funciona 
     */
    ADMUX &= 0b01000000;
    ADCSRA |= 0b01000000; //Inicia a conversao
    while (!(ADCSRA & 0b00010000))
        ;                //Espera finalizar
    return ADC;
}

uint8_t temperaturaConversor()
{

     //valorTemp =(2,5 *1024)/5
     /* Define a porta ADC1 como a que sera usada para conversao
     * Usa-se o operador '|' pois o unico bit que muda, com relacao 
     * ao adc0 eh o ultimo, que ficara '1' apos a logica com o 'ou'
     * Eh necessario tambem que seja definida a referencia, bit 6,
     * Pois, caso nao seja definida, a entrada nao funciona 
     */
    ADMUX |= 0b01000001;  //ADC1
    ADCSRA |= 0b01000000; //Inicia a conversao
    while (!(ADCSRA & 0b00010000))
        ; //Espera finalizar

   return ADC;
}
