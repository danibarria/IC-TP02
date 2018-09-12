
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX 20

int valores[MAX] = {1,2,3,4,5,6,7,8,9,0};

pthread_t hilo_productor;
pthread_t hilo_consumidor;

pthread_mutex_t p_mutex, c_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p_cond, c_cond = PTHREAD_COND_INITIALIZER;

int i_g, j ; 

// hacer de nuevo el programa productor consumidor con mutex y cond

void *consumidor(){
    pthread_mutex_lock(&c_mutex); //////////////////////////////c_mutex
    int l_j;
    for(l_j = 0 ; l_j < MAX ; l_j++ ){
        pthread_cond_wait(&c_cond, &c_mutex); //cede la ejjecucion del hilo luego vuelve   
        printf("CONSUMIDOR LOCK hilo:%d\n", valores[l_j]);    
    }   
    pthread_mutex_unlock(&c_mutex); //////////////////////////////c_mutex
    pthread_exit(NULL);
}

void *productor(){
    int i ;
    pthread_mutex_lock(&p_mutex);  /////////////////////////////////p_mutex
        for(i= 0; i < MAX ; i++){
            valores[i] = rand()%100+1;
            printf("PRODUCTOR LOCK hilo:%d\n", valores[i]); 
            i_g++; //cambio un valor valido en c_mutex
            pthread_cond_signal(&c_cond);  //tiene mas prioridad que la linea siguiente            
        }
    pthread_mutex_unlock(&p_mutex); ////////////////////////////////p_mutex
    pthread_exit(NULL);    
}

void main(){
    srand(time(NULL));
    i_g = -1; j = 0 ;    
    pthread_create(&hilo_productor, NULL, productor, NULL);
    pthread_create(&hilo_consumidor, NULL, consumidor, NULL);
    pthread_exit(NULL);
}