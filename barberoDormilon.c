#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_SILLAS 5

sem_t mutex, clientes, barbero;
int sillas_ocupadas = 0;

void *barbero(void *arg)
{
    while (1)
    {
        sem_wait(&clientes); // Espera a que llegue un cliente
        sem_wait(&mutex);    // Acceso exclusivo a las sillas
        sillas_ocupadas--;
        sem_post(&mutex); // Libera el acceso a las sillas
        // Corta el pelo (simulado con un sleep)
        printf("El barbero está cortando el pelo.\n");
        sleep(2);
        sem_post(&barbero); // Señal al siguiente cliente
    }
}

void *cliente(void *arg)
{
    sem_wait(&mutex); // Acceso exclusivo a las sillas
    if (sillas_ocupadas == NUM_SILLAS)
    {
        printf("Todas las sillas están ocupadas. El cliente se va.\n");
        sem_post(&mutex);
        pthread_exit(NULL);
    }
    else
    {
        sillas_ocupadas++;
        printf("Un cliente se ha sentado.\n");
        sem_post(&mutex);
        sem_post(&clientes); // Despierta al barbero
        sem_wait(&barbero);  // Espera a que el barbero termine
        printf("El cliente ha terminado.\n");
    }
}

int main()
{
    pthread_t tid_barbero, tid_clientes[NUM_SILLAS + 1];

    sem_init(&mutex, 0, 1);
    sem_init(&clientes, 0, 0);
    sem_init(&barbero, 0, 0);

    pthread_create(&tid_barbero, NULL, barbero, NULL);

    for (int i = 0; i <= NUM_SILLAS; i++)
    {
        pthread_create(&tid_clientes[i], NULL, cliente, NULL);
    }

    for (int i = 0; i <= NUM_SILLAS; i++)
    {
        pthread_join(tid_clientes[i], NULL);
    }

    pthread_cancel(tid_barbero);

    sem_destroy(&mutex);
    sem_destroy(&clientes);
    sem_destroy(&barbero);

    return 0;
}
