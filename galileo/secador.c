//bibliotecas padrão
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//bibliotecas do mraa
#include "mraa.h>"
#include "mraa/pwm.h" //necessário para pwm
#include "mraa/aio.h" //usada para pinos analogicos
#include "mraa/gpio.h" 

//configurações para uso dos pinos
#define A0 0   // pino A0 
#define A1 1  //pino A1
#define D12 12 //pino para o led do sistema
#define D8 8	//pino para o botão

#define timer 0       //variavel que controla o tempo de execução
#define intervalo 0  //retas que compoem a curva
#define valorLuz 0.0  //valor lido pelo sensor de luminosidade
#define valorTemp 0.0 //valor lido pelo sensor de temperatura
#define vmax 1023     //valor maximo lido pelos sensores 
#define estado 0      //variavel que indica funcionamento do sistema(ligado ou desligado)
#define curva 0.0     //valor a ser usado, somando a curva fixa com leitura dos sensores
#define passo 0.0     //variação do passo para o pwm
#define duty 0.0      //% a ser usada no pwm

volatile sig_atomic_t flag =1

void sig_handler(int signum){
	if(signum == SIGINT)
		fprintf(stdout,"Exiting...\n");
		flag=0;
	}
}

void *pwm(){
	mraa_init();
	mraa_pwm_context pwm,ledS1,ledS2; //ledS1 = led dos sensor de luminosidade, ledS2 = led do sensor de temperatura
	
	pwm = mraa_pwm_init(3); //inicia  a onda a ser usada no secador na D3
	ledS1= mraa_pwm_init(5); //inicia o led dos sensor de luz  na D5
	ledS2 = mraa_pwm_init(6); //inicia led do sensor de temp na D6
	
	//define periodo do pwm como 1s
	mraaa_pwm_period(pwm,1); 
	mraa_pwm_period(ledS1,1);
	mraa_pwm_period(ledS2,1);

	//habilita pwm nos pinos
	mraa_pwm_enable(pwm,1);
	mraa_pwm_enable(ledS1,1);
	mraa_pwm_enable(ledS2,1);
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
		if(intervalo == 0) //primeiro estado : curva iniciando
		{
	 		mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1, valorLuz);
			mraa_pwm_write(ledS2, valorTemp);
		}
		else if(intervalo ==1){
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1, valorLuz);
			mraa_pwm_write(ledS2,valorTemp );
		}
		else if(intervalo == 2){
			mraa_pwm_write(pwm,curva );
			mraa_pwm_write(ledS1,valorLuz );
			mraa_pwm_write(ledS2, valorTemp);
		}
		else if(intervalo == 3){
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1,valorLuz );
			mraa_pwm_write(ledS2, valorTemp);
		}
		else if(intervalo == 4){
			mraa_pwm_write(pwm,curva );
			mraa_pwm_write(ledS1, valorLuz);
			mraa_pwm_write(ledS2, valorTemp);
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
void *curva(){
	If(estado == 1){
		if(intervalo  == 0){
			if(passo < 30)
			passo= 3;
			duty+=passo;
		}
		else if(intervalo == 1){
			
		}
		
		
	}
		
}
void *contadorTempo(){
	while(timer <= 30){
		if(timer <= 10)
			intervalo =0;
		else if(timer>10 && timer <=15)
			intervalo = 1;
		else if(timer>15 && timer <=20)
			intervalo = 2;
		else if(timer > 20 && timer <= 25)
			intervalo = 3;
		else if(timer>25 && timer <= 30)
			intervalo =4;
		
		timer++;
	}
	if(timer == 30)
		timer = 0;
}
void setup(){
	mraa_init(); //inicializa mraa

	mraa_gpio_context led1,botao;
	led1 = mraa_gpio_init(Dx1); //led geral do sistema
	botao = mraa_gpio_init(Dx4);
	
	//define os led do sistema como saída
	mraa_gpio_dir(led1, MRAA_GPIO_OUT);
	//falta definir o botao como entrada
	
	//para escrever no pino, basta dizer qual é e o estado
	//mraa_gpio_write(led1,1);
	//para fechar o pino:
	//mraa_gpio_close(led1);
}
