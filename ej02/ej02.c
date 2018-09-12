
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>


#define CANT_HILOS 3

//Cree 3 hilos. El main deberá hacer join sobre el primer hilo. El primer hilo deberá
//esperar que se cumpla la condición de que un contador administrado por el segundo hilo
//llegue a 10. El segundo hilo solo podrá incrementar en uno su contador cuando el tercer
//hilo, que tiene a su vez un contador interno, llegue a 10. El tercer hilo tendrá llamadas a
//la función usleep a con valores aleatorios entre 100mS y 500ms (recuerde que debe
//multiplicar por 100 para llevar de mS a uS, necesario para la llamada a la función
//usleep).

pthread_mutex_t tercer_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t segundo_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t primer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t primer_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t segundo_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t intermedio = PTHREAD_COND_INITIALIZER;
pthread_cond_t tercer_cond = PTHREAD_COND_INITIALIZER;

int contador_s, contador_t;

void * contador_t_hilo(){
    int espera;
    pthread_mutex_lock(&tercer_mutex); ////////////////////////////////
    printf("tercero\n");
    
    for(int j = 1; j<=10; j++){
        for(int i = 1; i<=10; i++){
            espera = (rand()%400)+100;
            contador_t = i;
            printf("tercero ciclo for: %d , %d  espera: %d\n", i,j, espera);
        
            usleep(espera *100);
        }
        pthread_cond_signal(&intermedio);
    }
    
    pthread_mutex_unlock(&tercer_mutex); //////////////////////////////
    pthread_cond_signal(&segundo_cond);
}

void * contador_s_hilo(){
    pthread_mutex_lock(&segundo_mutex); ///////////////////////////////
    while(contador_t != 10){
        pthread_cond_wait(&intermedio, &segundo_mutex);  //lockea 2 
        contador_s++;
        printf("segundo ciclo for: %d\n", contador_s);
    }
    
    pthread_mutex_unlock(&segundo_mutex); /////////////////////////////
    pthread_cond_signal(&primer_cond);
    pthread_exit(NULL);
}

void * p_hilo_wait(){
    
    pthread_mutex_lock(&primer_mutex); ///////////////////////////////
    printf("primero\n");
    while(contador_s !=10){        
        pthread_cond_wait(&primer_cond, &primer_mutex); //lockea 1
    }
    pthread_mutex_unlock(&primer_mutex); /////////////////////////////
    

        printf("PRIMERO TERMINADO\n");
    pthread_exit(NULL);
}

void main(){

    srand(time(NULL));
    
    pthread_t p_hilo, s_hilo, t_hilo;
    
    contador_t = 1;
    contador_s = 0;
    
    pthread_create(&p_hilo,NULL, p_hilo_wait,NULL ); 
    
    pthread_create(&s_hilo,NULL, contador_s_hilo, NULL);
    pthread_create(&t_hilo, NULL, contador_t_hilo, NULL);
    pthread_join(p_hilo, NULL);
    pthread_exit(NULL);
}