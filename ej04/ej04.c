
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define HILOS 20
#define TAM_PILA 20

//Cree un programa que cree una pila concurrente:
//- Existe un conjunto de hilos que se determina al momento de ejecutar el programa que
//hacen PUSH sobre la pila.
//- Existe un conjunto de hilos que se determina al momento de ejecutar el programa que
//hacen POP sobre la pila. Si la pila está vacía, se bloquean, pero ni bien se hace PUSH, se
//despiertan.


int pila[TAM_PILA];
int pos_puntero = 0 ;

pthread_mutex_t mutex_push = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_pop = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= PTHREAD_COND_INITIALIZER;

int pila_vacia(){
    if(pila[0] == 0) {
        printf("Pila Vacia\n");
      return 1;  
    } else{
      return 0;  
    } 
 }

int pila_llena(){
    if(pila[TAM_PILA-1] == 1) {
        printf("Pila Llena");
        return 1;  
    }else{
        return 0;
    }
}

void *push(){
    pthread_mutex_lock(&mutex_push); //////////////////////////
    
    if(pila_llena()){
        sleep(2);
    }else{
        printf(":push:\n");
        pila[pos_puntero] = 1 ;   //posicion de pila ocupada
        pos_puntero++;            //aumenta la posicion del tope
        pthread_cond_signal(&cond);
    }
        
    pthread_mutex_unlock(&mutex_push); /////////////////////////
    pthread_exit(NULL);
}
void *pop(){
    pthread_mutex_lock(&mutex_push); //////////////////////////////
    
    while(pila_vacia())
            pthread_cond_wait(&cond, &mutex_push);
        
    printf(":pop:\n");
    printf("%d :: ", pos_puntero);
    pila[pos_puntero] = 0;   // posicion en la pila libre
    pos_puntero--;           // disminuye la posicion del tope
    printf("%d\n", pos_puntero);

    pthread_mutex_unlock(&mutex_push); //////////////////////////////
    pthread_exit(NULL);
}



int main() {
    int i;

    pthread_t hilos_push[HILOS];
    pthread_t hilos_pop[HILOS];

    pos_puntero = 0;
    
    for(i=0; i<TAM_PILA; i++){
        pila[i] = 0;
    }
    
    for (i=0; i<HILOS; i++) {
        pthread_create(&hilos_push[i], NULL, &push, NULL);
    }
        for (i=0; i<HILOS; i++) {
        pthread_create(&hilos_pop[i], NULL, &pop, NULL);
    }
 
    for (i=0; i<HILOS; i++) {
        pthread_join(hilos_push[i], NULL);
    }
        for (i=0; i<HILOS; i++) {
        pthread_join(hilos_pop[i], NULL);
    }
    return 0;
}