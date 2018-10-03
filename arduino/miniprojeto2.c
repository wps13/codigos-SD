#include <avr/io.h>
#include <avr/delay.h>

int ldrConversor();
int temperaturaConversor();

void setup()
{
    OCR0A = 0b1000000; //64 = 25%
    //controla quando deve ocorrer interrupcao
    //conta os pulsos de clock, onde top eh o topo da onda
    //sendo seu valor maximo = 255
    //habilita interrupcao com a comparacao com OCR0A, que armazena a largura do pulso de clock
    //Os dois primeiros bits servem para definir(set) o OC0A
    //quando em modo de comparacao e limpar(clear) ao descer
    //os dois pinos seguintes modificam a conf do 0C0B
    //dois seguintes sao reservados
    //os ultimos definem modo de operaçao
    //01 = pwm, 8 bits, att no topo e flag na descida
    TCCR0A = 0b11000001;
    //modo de correção de fase do pwm, considerando o prescaling em 1024 e fclock = 16MHz
    TCCR0B = 0b00001101;
    //configurações iniciais do microcontrolador
    DDRB = 0b00100000;   //define pino PB5(13) como saida
    PORTB = 0b00000000;  //coloca as portas em 'low'
    ADMUX = 0b01000000;  //seleciona AVCC como referencia
    ADCSRA = 0b10000111; //habilita adc e fator de divisao da f = 128
}
void loop()
{
    int resultLdr, resultTemp;
    uint8_t overflow = 0;

    //quantidade necessaria de ciclos de clock para atingir 30 s
    //fClockCiclo = fosc/ (2*prescaler*top), foi usado top = 127.5
    //TporClock = 16.32 ms
    //qntdOFLed = 30 s / 16.32 ms
    int qntdOFLed = 1838;

    //DEFINIR CONDIÇÃO DO LOOP
    while (1)
    {

        /* Esse loop eh usado para verificar, de forma continua, as condicoes especificadas
     * fazendo as conversoes das leituras analogicas, retornando um 'true'
     * e jogando pro led, atraves do PORTB
     */
        resultTemp = temperaturaConversor();
        resultLdr = ldrConversor();
        while (overflow >= qntdOFLed)
        {
            TIMSK0 = 0b00000010;  //primeiros bits sao reservados
            TCNT0++;             //registrador que armazena que armazena o timer
            TIFR0 = 0b00000010;  //Avisa que ocorreu a interrupcao
            PORTB |= 0b00100000; //liga led
            _delay_ms(100);
            PORTB &= 0b00000000; // apaga led
            overflow++;
        }
        TCNT0 &= 0b0000000; //zerar o registrador
        overflow = 0;

        if (resultLdr && resultTemp)
            PORTB |= 0b00100000; //Aciona PB5(13)
    }
}
int ldrConversor()
{
    /* O valorLDR eh dependente da tensao que fica sobre o tensor, 
     * após a divisão de tensao, onde aqui seria com resistores de 10k
     *  Com o valor da tensao obtido, usaria a fórmula da resolucao
     * Tensão de entrada = 3
     * valorLDR = (3*1024)/5
     */
    int valorLDR = 614;

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
    if (ADC >= valorLDR) //Checa o valor de tensao lido
        return 1;
    return 0;
}

int temperaturaConversor()
{

    //CALCULAR TENSAO A SER USADA COMO REFERENCIA
    //valorTemp =(2,5 *1024)/5
    int valorTemp = 512;
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

    //Checa se a tensao obtida eh maior que a considerada como referencia
    if (ADC >= valorTemp)
        return 1; //checa
    return 0;
}
