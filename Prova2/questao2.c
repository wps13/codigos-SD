/* Questão 2
 *
 *Troca de informações entre a galileo e dois atmega 
 * usando o protocolo SPI
 */

//bibliotecas padrao
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "mraa/spi.h" 
#include "mraa/gpio.h" 
#define SPI_BUS 0 //declaração do spi
#define SPI_FREQ 400000 //frequência do SPI em HZ

void main(){
	mraa_init();

	mraa_spi_context spi;
	spi = mraa_spi_init(SPI_BUS); //inicializa spi bus
	mraa_spi_frequency(spi,SPI_FREQ); //seta frequência do spi

	mraa_gpio_context arduino1, arduino2;
	arduino1 = mraa_gpio_init(2); //inicia na D2
	arduino2 = mraa_gpio_init(4); //inicia na D4
	//coloca apos os pinos como saida para enviar dados
	mraa_gpio_dir(arduino1, MRAA_GPIO_OUT); 
	mraa_gpio_dir(arduino2, MRAA_GPIO_OUT); 

	while(1){
		//iniciando comunicação, ao colocar o arduino como escravo
		mraa_gpio_write(arduino1,0);
		mraa_spi_write_word(spi,1);//envia dado
		mraa_gpio_write(arduino1,1);//finaliza comunicação
		//colocar arduino em modo escravo
		mraa_gpio_write(arduino2,0);
		mraa_spi_write_word(spi,0);//envia o dado
		mraa_gpio_write(arduino2,1);;//finaliza
		
	}
