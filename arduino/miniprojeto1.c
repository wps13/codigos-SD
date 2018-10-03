#include <avr/io.h>

int ldrConversor();
int temperaturaConversor();

void setup(){

    //configurações iniciais do microcontrolador
    DDRB = 0b00100000;   //define pino PB5(13) como saída
    PORTB = 0b00000000;  //coloca as portas em 'low'
    ADMUX = 0b01000000; //seleciona AVCC como referência
    ADCSRA = 0b10000111; //habilita adc e fator de divisão da f = 128

}
void loop()
{
    int resultLdr, resultTemp;

    //DEFINIR CONDIÇÃO DO LOOP
    while(1){

    /* Esse loop é usado para verificar, de forma contínua, as condições especificadas
     * fazendo as conversões das leituras analógicas, retornando um 'true'
     * e jogando pro led, atráves do PORTB
     */
    resultTemp = temperaturaConversor();
    resultLdr = ldrConversor();

    if (resultLdr && resultTemp)
        PORTB |= 0b00100000; //Aciona PB5(13)
    }   
}


int ldrConversor(){
    /* O valorLDR é dependente da tensão que fica sobre o tensor, 
     * após a divisão de tensão, onde aqui seria com resistores de 10k
     *  Com o valor da tensão obtido, usaria a fórmula da resolução
     * Tensão de entrada = 3
     * valorLDR = (3*1024)/5
     */
    int valorLDR = 614;

    /* Define a porta ADC0 como a que será usada para conversão
     * Usa-se o operador '&' devido a necessidade de uso de bits 0
     * Caso esteja com o bit '1' em alguma porta, isso zera todas
     * possibilitando assim que a porta definida seja usada,
     * Uma vez que o código para o ADC0 é 0000.
     * É necessário também que seja definida a referência, bit 6,
     * Pois, caso não seja definida, a entrada não funciona 
     */
    ADMUX &= 0b01000000;
    ADCSRA |= 0b01000000; //Inicia a conversão
    while (!(ADCSRA & 0b00010000)); //Espera finalizar
    if (ADC >= valorLDR) //Checa o valor de tensão lido
        return 1;
    return 0;
}

int temperaturaConversor(){

    //CALCULAR TENSÃO A SER USADA COMO REFERÊNCIA
    //valorTemp =(2,5 *1024)/5
    int valorTemp = 512;
     /* Define a porta ADC1 como a que será usada para conversão
     * Usa-se o operador '|' pois o único bit que muda, com relação 
     * ao adc0 é o último, que ficará '1' apõs a lógica com o 'ou'
     * É necessário também que seja definida a referência, bit 6,
     * Pois, caso não seja definida, a entrada não funciona 
     */
    ADMUX |= 0b01000001; //ADC1
    ADCSRA |= 0b01000000; //Inicia a conversão
    while (!(ADCSRA & 0b00010000)); //Espera finalizar

    //Checa se a tensão obtida é maior que a considerada como referência
    if (ADC >= valorTemp) return 1;//checa
    return 0;
}
