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
#define SPI_BUS 0 //declaração do spi
#define SPI_FREQ 400000 //frequência do SPI em HZ

void main(){
	mraa_spi_context spi;
	spi = mraa_spi_init(SPI_BUS); //inicializa spi bus
	mraa_spi_frequency(spi,SPI_FREQ); //seta frequência do spi
	mraa_init();

	// mraa_spi_write_word(pino,valor);
	//mraa_spi_write_buf_word(par1,par2,par3);
	
	mraa_spi_stop(spi);
	mraa_deinit();


