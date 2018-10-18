//bibliotecas padrao
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//bibliotecas do mraa
#include "mraa.h"
#include "mraa/pwm.h" //necessario para pwm
#include "mraa/aio.h" //usada para pinos analogicos
#include "mraa/gpio.h"//usada para os pinos digitais
#include "pthread.h" //usada para os threads

//configuracoes para uso dos pinos
#define A0 0   // pino A0 para sensor de luminosidade
#define A1 1   //pino A1 para sensor de temperatura
#define D12 12 //pino para o led do sistema
#define D8 8   //pino para o botao
#define vmax 1023	 //valor maximo lido pelos sensores

int timer= 0	;	  //variavel que controla o tempo de execução
int intervalo =0;   //retas que compoem a curva
float valorLuz=0.0; //valor lido pelo sensor de luminosidade
float valorTemp=0.0; //valor lido pelo sensor de temperatura
int estado= 0;  //variavel que indica funcionamento do sistema(ligado ou desligado)
float valorCurva =0.0;	 //valor a ser usado, somando a curva fixa com leitura dos sensores
float passo =0.0;	 //variacao do passo para o pwm
float duty= 0.0;	  //% a ser usada no pwm
int estadoBotao= 0;  //botao pressionado ou nao

volatile sig_atomic_t flag = 1;

	void sig_handler(int signum)
	{
	if (signum == SIGINT)
		fprintf(stdout, "Exiting...\n");
	flag = 0;
	}

void *pwm()
{
	mraa_init();
	mraa_pwm_context pwm, ledS1, ledS2; //ledS1 = led dos sensor de luminosidade, ledS2 = led do sensor de temperatura

	pwm = mraa_pwm_init(3);   //inicia  a onda a ser usada no secador na D3
	ledS1 = mraa_pwm_init(5); //inicia o led dos sensor de luz  na D5
	ledS2 = mraa_pwm_init(6); //inicia led do sensor de temp na D6

	//define periodo do pwm como 1s
	mraaa_pwm_period(pwm, 1);
	mraa_pwm_period(ledS1, 1);
	mraa_pwm_period(ledS2, 1);

	//habilita pwm nos pinos
	mraa_pwm_enable(pwm, 1);
	mraa_pwm_enable(ledS1, 1);
	mraa_pwm_enable(ledS2, 1);

	//Checa se pwm foi inicializado
	if (pwm == NULL)
	{
		fprintf(stderr, "Inicialização do pwm falhou.\n");
		mraa_deinit();
		return EXIT_FAILURE;
	}

	while (estado)
	{
		//cada intervalo eh referente a uma reta do grafico 
		//assim, eh possivel controlar em qual parte encontra-se
		//a execucao e escrever o valor de pwm, considerando
		//a curva inicial e a leitura dos sensores
		if (intervalo == 0) //primeiro estado : curva iniciando
		{
			mraa_pwm_write(pwm, valorCurva);
			mraa_pwm_write(ledS1,valorCurva );
			mraa_pwm_write(ledS2,valorCurva );
		}
		else if (intervalo == 1)
		{
			mraa_pwm_write(pwm, valorCurva);
			mraa_pwm_write(ledS1, valorCurva);
			mraa_pwm_write(ledS2, valorCurva);
		}
		else if (intervalo == 2)
		{
			mraa_pwm_write(pwm, valorCurva);
			mraa_pwm_write(ledS1, valorCurva);
			mraa_pwm_write(ledS2, valorCurva);
		}
		else if (intervalo == 3)
		{
			mraa_pwm_write(pwm, valorCurva);
			mraa_pwm_write(ledS1, valorCurva);
			mraa_pwm_write(ledS2, valorCurva);
		}
		else if (intervalo == 4)
		{
			mraa_pwm_write(pwm, valorCurva);
			mraa_pwm_write(ledS1,valorCurva);
			mraa_pwm_write(ledS2, valorCurva);
		}

	err_exit:
		mraa_pwm_close(pwm);
		mraa_pwm_close(ledS1);
		mraa_pwm_close(ledS2);
		mraa_deinit();
		return EXIT_FAILURE;
	}
	pthread_exit((void *)1);
}

	//funcao que lida com os sensores
	void *aio()
	{

		mraa_result_t status = MRAA_SUCCESS;
		mraa_aio_context sensorLuz, sensorTemp;
		sensorLuz = mraa_aio_init(A0);  //inicializa o sensor LDR na A0
		sensorTemp = mraa_aio_init(A1); //inicializa A1 como Temperatura

		valorLuz = mraa_aio_read(sensorLuz);
		valorTemp = mraa_aio_read(sensorTemp);

		//conversao dos valores lidos nos sensores para obtencao de valores
		//humanamente compreensiveis
		if (valorTemp >= 900)
			valorTemp = (1 - (valorTemp / vmax)) * 400;
		else
			valorTemp = (valorTemp / vmax) * 400;
		valorLuz /= vmax;
		pthread_exit((void *)1);
	}
	//funcao responsavel por gerar a curva a ser usada no pwm
	void *curva()
	{
		If(estado == 1)
		{
			if (intervalo == 0)
			{
				passo = 3;
				while(duty < 30){
					duty += passo;
					valorCurva = duty + valorLuz + valorTemp;
				}
			}
			else if (intervalo == 1)
			{
				valorCurva = 30 + valorLuz + valorTemp;
			}
			else if(intervalo == 2){
				passo = 5;
				while(duty < 65){
					duty += passo;
					valorCurva = duty + valorLuz + valorTemp;
				}
			}
			else if(intervalo == 3){
				valorCurva = 30 + valorLuz + valorTemp;
			}
			else if(intervalo == 4){
				passo = 13;
				while(duty > 0){
					duty -= passo;
					valorCurva = duty + valorLuz + valorTemp;
				}
			}

		}
		pthread_exit((void *)1);
	}
	//funcao que controla o tempo de execucao
	// e define em qual intervalo encontra-se
	void *contadorTempo()
	{
		while (timer <= 30)
		{
			if (timer <= 10)
				intervalo = 0;
			else if (timer > 10 && timer <= 15)
				intervalo = 1;
			else if (timer > 15 && timer <= 20)
				intervalo = 2;
			else if (timer > 20 && timer <= 25)
				intervalo = 3;
			else if (timer > 25 && timer <= 30)
				intervalo = 4;

			timer++;
		}
		if (timer == 30)
			timer = 0;
		pthread_exit((void *)1);
	}
	void *threadCliente(){
		struct sockaddr_in servidor, cliente;
		int sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock == -1){
		perror("socket ");
		exit(1);
		}
		cliente.sin_family = AF_INET;
		cliente.sin_port = htons(porta);
		cliente.sin_addr.s_addr = inet_addr("10.13.111.32"); 
		memset(cliente.sin_zero, 0, 8);

		if(bind(sock, (struct sockaddr*) &servidor, sizeof(servidor)) == -1){
		perror("bind ");
		exit(1);
		}

		while(1){
			sendto(sock,&valorCurva,sizeof(valorCurva),0,(struct sockaddr*) &cliente, sizeof(cliente));
			sleep(1);
		}
		pthread_exit((void *)1);
	}

	void setup()
	{
		mraa_init(); //inicializa mraa

		//threads a serem usadas
		pthread_t pwmSaida,valoresCurva,contagemTempo,sensores,cliente;
		//cria as threads com os valores padrao
		
		mraa_gpio_context led1, botao;
		led1 = mraa_gpio_init(D12); //led geral do sistema
		botao = mraa_gpio_init(D8);
		
		//define os led do sistema como saída
		mraa_gpio_dir(led1, MRAA_GPIO_OUT);
		//define o botao como entrada
		mraa_gpio_dir(botao, MRAA_GPIO_IN);
		//garante que o led esteja desligado inicialmente	
		mraa_gpio_write(led1, 0); 
		
		estadoBotao = mraa_gpio_read(botao);

		while(1){
			//o botao eh pressionado quando for lido 0
			if(!estadoBotao)//botao sendo pressionado
			{
				estado = 1;//sistema deve ser ligado
			}
			//inicia o sistema
			else if(estado){
				//indica que o sistema esta ligado
				mraa_gpio_write(led1,1);
				//thread para o pwm a ser usado no ventilador e leds
				pthread_create(&pwmSaida,NULL,pwm,NULL);
				//thread para a geracao da curva
				pthread_create(&valoresCurva,NULL,curva,NULL);
				//thread para a contagem do tempo
				pthread_create(&contagemTempo,NULL,contadorTempo,NULL);
				//thread para os sensores
				pthread_create(&sensores,NULL,aio,NULL);
				//thread para o cliente
				pthread_create(&cliente,NULL,threadCliente,NULL);
			}
		}
	}
