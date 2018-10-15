//bibliotecas padrão
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//bibliotecas do mraa
#include "mraa.h"
#include "mraa/pwm.h" //necessário para pwm
#include "mraa/aio.h" //usada para pinos analogicos
#include "mraa/gpio.h"//usada para os pinos digitais
#include "pthread.h" //usada para os threads

//configurações para uso dos pinos
#define A0 0   // pino A0 para sensor de luminosidade
#define A1 1   //pino A1 para sensor de temperatura
#define D12 12 //pino para o led do sistema
#define D8 8   //pino para o botão

#define timer 0		  //variavel que controla o tempo de execução
#define intervalo 0   //retas que compoem a curva
#define valorLuz 0.0  //valor lido pelo sensor de luminosidade
#define valorTemp 0.0 //valor lido pelo sensor de temperatura
#define vmax 1023	 //valor maximo lido pelos sensores
#define estado 0	  //variavel que indica funcionamento do sistema(ligado ou desligado)
#define curva 0.0	 //valor a ser usado, somando a curva fixa com leitura dos sensores
#define passo 0.0	 //variação do passo para o pwm
#define duty 0.0	  //% a ser usada no pwm
#define estadoBotao 0  //botão pressionado ou nao

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
		//cada intervalo é referente a uma reta do grafico 
		//assim, é possível controlar em qual parte encontra-se
		//a execução e escrever o valor de pwm, considerando
		//a curva inicial e a leitura dos sensores
		if (intervalo == 0) //primeiro estado : curva iniciando
		{
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1,curva );
			mraa_pwm_write(ledS2,curva );
		}
		else if (intervalo == 1)
		{
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1, curva);
			mraa_pwm_write(ledS2, curva);
		}
		else if (intervalo == 2)
		{
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1, curva);
			mraa_pwm_write(ledS2, curva);
		}
		else if (intervalo == 3)
		{
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1, curva);
			mraa_pwm_write(ledS2, curva);
		}
		else if (intervalo == 4)
		{
			mraa_pwm_write(pwm, curva);
			mraa_pwm_write(ledS1,curva);
			mraa_pwm_write(ledS2, curva);
		}

	err_exit:
		mraa_pwm_close(pwm);
		mraa_pwm_close(ledS1);
		mraa_pwm_close(ledS2);
		mraa_deinit();
		return EXIT_FAILURE;
	}
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
					curva = duty + valorLuz + valorTemp
				}
			}
			else if (intervalo == 1)
			{
				curva = 30 + valorLuz + valorTemp;
			}
			else if(intervalo == 2){
				passo = 5;
				while(duty < 65){
					duty += passo;
					curva = duty + valorLuz + valorTemp
				}
			}
			else if(intervalo == 3){
				curva = 30 + valorLuz + valorTemp;
			}
			else if(intervalo == 4){
				passo = 13
				while(duty > 0){
					duty -= passo;
					curva = duty + valorLuz + valorTemp
				}
			}

		}
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
	}
	void setup()
	{
		mraa_init(); //inicializa mraa

		//threads a serem usadas
		pthread_t pwmSaida,valoresCurva,contagemTempo,sensores;
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
				//INICIAR THREADS AQUI
				//thread para o pwm a ser usado no ventilador e leds
				pthread_create(&pwmSaida,NULL,pwm,NULL);
				//thread para a geracao da curva
				pthread_create(&valoresCurva,NULL,curva,NULL);
				//thread para a contagem do tempo
				pthread_create(&contagemTempo,NULL,contadorTempo,NULL);
				//thread para os sensores
				pthread_create(&sensores,NULL,aio,NULL);
			}
		}
	}
