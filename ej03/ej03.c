
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define CANT_HILOS 10

//Cree 10 hilos que atiendan a una misma variable de condición. Despierte a todos los
//hilos con una llamada a biblioteca (averigue cual es). Imprima el instante en el que se
//despiertan los hilos.

pthread_mutex_t mutexe = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER;

int contador = 0;

void control(){
    pthread_mutex_lock(&mutexe);
    
    printf("hilo mutex %d contador: %d \n", pthread_self(), contador);
    while(contador != 10)
        pthread_cond_wait(&var_cond, &mutexe);
        
        
    printf("hilo %d despertado \n", pthread_self());
    pthread_mutex_unlock(&mutexe);
    pthread_exit(NULL);
}

void sumador(){
    int espera;
    pthread_mutex_lock(&mutex_s);
    
    while(contador !=10){
        espera = (rand()%500)+100;
        usleep(espera *100);
        printf("[espera: %d %d \n", espera, contador);
        contador++;
        
    }
    pthread_mutex_unlock(&mutex_s);
    pthread_cond_broadcast(&var_cond);
    pthread_exit(NULL);
}

void main(){
    pthread_t hilos[CANT_HILOS];
    pthread_t h_contador;
    int cant_hilos = CANT_HILOS;
    
    for(int i= 0; i < cant_hilos; i++){  //crea 10 hilos 
        pthread_create(&hilos[i],NULL, (void *)control, NULL );
    }
    pthread_create(&h_contador, NULL, (void *)sumador, NULL);
    
    pthread_join(h_contador, NULL);
    pthread_exit(NULL);
    
}