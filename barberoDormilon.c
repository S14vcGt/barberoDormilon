#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
// Número de clientes
#define MAX_CLIENTES 25
void *cliente(void *num);
void *barbero(void *num);
// Limitar acceso a las sillas de espera al mismo tiempo
sem_t SillaEspera;
// Acceso a la silla del barbero
sem_t barberosilla;
// barbero duerme mientras no hay cliente
sem_t barberodormir;
// El cliente debe esperar hasta que el barbero termine de cortar su cabello
sem_t CorteCabello;
// Bandera para detener el hilo del barbero cuando todos los hilos han sido atendidos
int TodosAtendidos = 0;
int main(int argc, char *argv[])
{
    pthread_t btid;
    pthread_t tid[MAX_CLIENTES];
    // long RandSeed;
    int i, numclientes, numsillas;
    int Number[MAX_CLIENTES];

    if (argc != 3)
    {
        printf("Use: barberoDormilon <Num clientes> <Num sillas>\n");
        exit(-1);
    }

    // Toma los argumentos
    numclientes = atoi(argv[1]);
    numsillas = atoi(argv[2]);

    // El número de threads es menor al número de clientes que soporta
    if (numclientes > MAX_CLIENTES)
    {
        printf("El número máximo de clientes es %d.\n", MAX_CLIENTES);
        exit(-1);
    }
    // Inicializar el arreglo
    for (i = 0; i < MAX_CLIENTES; i++)
    {
        Number[i] = i;
    }

    // Valores iniciales de semáforos
    sem_init(&SillaEspera, 0, numsillas);
    sem_init(&barberosilla, 0, 1);
    sem_init(&barberodormir, 0, 0);
    sem_init(&CorteCabello, 0, 0);

    // Crear thread del barbero
    pthread_create(&btid, NULL, barbero, NULL);

    // Crear thread de los clientes
    for (i = 0; i < numclientes; i++)
    {
        pthread_create(&tid[i], NULL, cliente, (void *)&Number[i]);
    }

    // Une los threads al finalizar
    for (i = 0; i < numclientes; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Al finalizar todos los clientes
    TodosAtendidos = 1;
    sem_post(&barberodormir); // Despierta el barbero para cerrar la barbería
    pthread_join(btid, NULL);
}
void *cliente(void *number)
{
    int num = *(int *)number;

    randwait(1000 + num);
    printf("El cliente %d llegó a la barbería\n", num + 1);

    if (sem_trywait(&SillaEspera) != 0)
    {
        printf("Todas las sillas estan ocupadas, el cliente %d se va\n", num + 1);
        pthread_exit(NULL);
    }
    else
    {
        // Espera para sentarse
        sem_wait(&SillaEspera);
        printf("El cliente %d se sienta en la sala de espera\n", num + 1);

        // Espera que se levante el barbero de su silla
        sem_wait(&barberosilla);

        // El cliente desocupa su silla de la sala de espera
        sem_post(&SillaEspera);

        // Se levanta el barbero
        printf("El cliente %d levanta al barbero\n", num + 1);
        sem_post(&barberodormir);

        // Espera que el barbero le corte su cabello
        sem_wait(&CorteCabello);

        // Deja su silla del barbero
        sem_post(&barberosilla);
        printf("El cliente %d se fué\n", num + 1);
    }
    return 0;
}
void *barbero(void *num)
{
    while (!TodosAtendidos)
    {
        printf("El barbero está durmiendo\n");
        sem_wait(&barberodormir);
        if (!TodosAtendidos)
        {
            printf("El barbero está cortando cabello\n");
            // Terminó de hacer el corte
            sem_post(&CorteCabello);
            printf("Ya terminó\n");
        }
    }
    printf("El barbero salió de la barbería\n");
    return 0;
}

void randwait(int max_wait)
{
    // Inicializar el generador de números aleatorios
    srand(time(NULL));

    // Generar un número aleatorio entre 0 y max_wait
    int wait_time = rand() % max_wait;

    // Esperar el tiempo aleatorio en milisegundos
    usleep(wait_time * 1000);
}