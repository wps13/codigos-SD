//Questão 1
// Implementar um somatório de wi*xi, 0<=i<=4
//xi é enetrada analógica
//wi é o ganho, que é um valor aleatório entre 0 e 1
//usar a intel Galileo para implementar

//bibliotecas padrao
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//biblioteca para manipular a galileo
#include "mraa/aio.h"
#include "mraa/pwm.h"

//bibliotecas para gerar os nums. aleatorios
#include <stdio.h>
#include <time.h>

float x[5]; //vetor para armazenar as leituras dos pinos
float w[5]; //vetor para armazenar os ganhos

float aleatorio(){
	float rnum = 0.0;
	srand((unsigned)time(NULL));
	
	return rnum=(float)(rand()/RAND_MAX;
}
//função para armazenar a leitura das entradas analógicas em vetor
void aio(){
	mraa_init();
	mraa_aio_context x0,x1,x2,x3,x4;

	//definição dos pinos a serem usados como analógicos
	x0 = mraa_aio_init(0);
	x1 = mraa_aio_init(1);
	x2 = mraa_aio_init(2);
	x3 = mraa_aio_init(3);
	x4 = mraa_aio_init(4);

	//leitura dos pinos
	x[0] = mraa_aio_read(x0);
	x[1]= mraa_aio_read(x1);
	x[2] = mraa_aio_read(x2);
	x[3]= mraa_aio_read(x3);
	x[4] = mraa_aio_read(x4);

}

//gera o vetor de pesos aleatórios entre 0 e 1
void ganhos(){
	for(int i=0;i<5;i++)
		w[i]=aleatorio();
}

void main(){
	mraa_init();
	mraa_pwm_context y;
	y = mraa_pwm_init(3);//inicia pwm na D3
	mraa_pwm_period(y,1); //1s como periodo do pwm	
	mraa_pwm_enable(y,1);//habilita pwm no pino
	
	float saida=0.0;
	for(int i;i<5;i++){
		saida +=  w[i]*x[i];
	}
	mraa_pwm_write(y,saida);
}
