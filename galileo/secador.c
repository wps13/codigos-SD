//bibliotecas padrão
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//bibliotecas do mraa
#include "mraa.h>"
#include "mraa/pwm.h" //necessário para pwm
#include "mraa/aio.h" //usada para pinos analogicos
#include "mraa/gpio.h" 

//configurações para uso
#define PWM 3
#define A0 0   // pino A0
#define A1 1  //pino A1
#define Dx1 //escolher pinos para os leds e o botão(Dx, onde x é o num)
#define Dx4

#define timer 0
#define ciclos 0
#define valorLuz 0.0
#define valorTemp 0.0
#define vmax 1023
#define estado 0
volatile sig_atomic_t flag =1

void sig_handler(int signum){
	if(signum == SIGINT)
		fprintf(stdout,"Exiting...\n");
		flag=0;
	}
}

void *pwm(){
	mraa_init();
	mraa_pwm_context pwm,ledS,ledD; //ledS = led dos sensores, ledD = led geral do sistema
	
	pwm = mraa_pwm_init(3); //inicia  a onda a ser usada no secador na D3
	ledS= mraa_pwm_init(5); //inicia o led dos sensores na D5
	ledD = mraa_pwm_init(6); //inicia led do sistema na D6


	//Checa se pwm foi inicializado
	if(pwm == NULL){
		fprintf(stderr, "Inicialização do pwm falhou.\n");
		mraa_deinit();
		return EXIT_FAILURE;
	}
	
	
	while(estado){
	//cada estado seria referente a curva gerada
	//onde é necessário definir a duração dos ciclos com o timer
	//manipular os pwms e suas intensidades
	//usando mraa_pwm_write(pino,valor)
		if(ciclos == 0) //primeiro estado : curva iniciando
		{
	 		mraa_pwm_write(pwm, );
			mraa_pwm_write(ledS, );
			mraa_pwm_write(ledD, );
		}
		else if(ciclos ==1){

		}
		else if(ciclos == 2){
		}
		else if(ciclos == 3){
		}
		else if(ciclos == 4){
		}	



err_exit:
	mraa_result_print(status);
	mraa_pwm_close(pwm);
	mraa_pwm_close(ledS);
	mraa_pwm_close(ledD);
	mraa_deinit();
	return EXIT_FAILURE;

}

//função que lida com os sensores
void *aio(){

	mraa_result_t status = MRAA_SUCCESS;
	mraa_aio_context sensorLuz, sensorTemp;
	sensorLuz = mraa_aio_init(A0); //inicializa o sensor LDR na A0
	sensorTemp = mraa_aio_init(A1); //inicializa A1 como Temperatura

	valorLuz = mraa_aio_read(sensorLuz);
	valorTemp = mraa_aio_read(sensorTemp);
	
	//divide-se o valor lido por 1023, por conta do valor máximo que é lido no sensor analogico
	//sendo obtido ~0.3 na luminosidade maior e ~0.7 no escuro 
	valorLuz /= vmax;
	valorTemp /= vmax;
}
void setup(){
	mraa_init(); //inicializa mraa

	mraa_gpio_context led1,led2,led3,botao;
	led1 = mraa_gpio_init(Dx1);
	botao = mraa_gpio_init(Dx4);
	
	//define os led do sistema como saída
	mraa_gpio_dir(led1, MRAA_GPIO_OUT);
	
	//para escrever no pino, basta dizer qual é e o estado
	//mraa_gpio_write(led1,1);
	//para fechar o pino:
	//mraa_gpio_close(led1);


