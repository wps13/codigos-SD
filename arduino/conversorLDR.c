#include <avr/io.h>

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
