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
#define PWM_FREQ 200 //pwm period in us
#define A0 0   // pino A0
#define A1 1  //pino A1
#define led1 //escolher pinos para os leds e o botão(Dx, onde x é o num)
#define led2
#define led3
#define botao

volatile sig_atomic_t flag =1

void sig_handler(int signum){
	if(signum == SIGINT)
		fprintf(stdout,"Exiting...\n");
		flag=0;
	}
}

void pwm(){
	mraa_init();
	mraa_result_t status = MRAA_SUCCESS;
	mraa_pwm_context pwm;
	float value=0.0f;
	float output;

	pwm = mraa_pwm_init(pwm);

	//Checa se pwm foi inicializado
	if(pwm == NULL){
		fprintf(stderr, "Inicialização do pwm falhou.\n");
		mraa_deinit();
		return EXIT_FAILURE;
	}

	status = mraa_pwm_period_us(pwm, PWM_FREQ); //periodo do pwm
	if(status != MRAA_SUCCESS)
		goto err_exit;
	while(flag){
		value += 0.01f;
		status = mraa_pwm_write(pwm,value);
		if(status != MRAA_SUCCESS)
			goto err_exit;

		usleep(50000);

		if(value >= 1.0f)
			value=0.0f;

		output = mraa_pwm_read(pwm);
		fprintf(stdout,"pwm value is %f\n", output);
	}
	mraa_pwm_close(pwm);

err_exit:
	mraa_result_print(status);
	mraa_pwm_close(pwm);
	mraa_deinit();
	return EXIT_FAILURE;
}

void aio(){
	mraa_result_t status = MRAA_SUCCESS;
	mraa_aio_context sensorLuz, sensorTemp;
	sensorLuz = mraa_aio_init(A0); //inicializa o sensor LDR na A0
	sensorTemp = mraa_aio_init(A1); //inicializa A1 como Temperatura
	valorLuz = mraa_aio_read(sensorLuz);
	valorTemp = mraa_aio_read(sensorTemp);

	//fechar os pinos
	mraa_aio_close(sensorLuz);
	mraa_aio_close(sensorTemp);
}

void setup(){
	mraa_init(); //inicializa mraa
	pwm();
	aio();

