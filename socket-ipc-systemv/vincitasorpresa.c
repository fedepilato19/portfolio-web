#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_STRING_LENGTH 100
#define MAX_GIOIELLI 10
#define MSG_KEY 12345
#define MSG_TYPE 1

int semid; //identificatore del semaforo
int msgid; //identificatore della coda di messaggi

//struttura che specifica i dati del messaggio che invia al server
typedef struct{
    long type;
    char gadget[MAX_STRING_LENGTH];
} MsgData;

//struttura per la creazione di semafori system-V
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void f_bracciale(int s);
void f_collana(int s);
void f_stop(int s);

int main() 
{
    signal(SIGUSR1, f_bracciale);
    signal(SIGUSR2, f_collana);
    signal(SIGTERM, f_stop);
    
    srand((unsigned int)time(NULL));
    int numero_random = rand() % MAX_GIOIELLI;

    // Creazione del semaforo System V
    key_t key = ftok(".", 'x');
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Errore nella creazione del semaforo");
        exit(1);
    }

    // Inizializzazione del semaforo a 1
    union semun su;
    su.val = 1;
    if (semctl(semid, 0, SETVAL, su) == -1) 
    {
        perror("Errore nell'inizializzazione del semaforo");
        exit(1);
    }
    
    // Creazione della coda di messaggi
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    
    if (msgid == -1) 
    {
        perror("Errore nella creazione/apertura della coda di messaggi");
        exit(EXIT_FAILURE);
    }

    while (1) 
    {
        printf("\nEstrazione in corso...\n");
        printf("Numero generato: %d\n", numero_random);
       
        
        if (numero_random > 5) 
        {
          printf("Mi dispiace, non sei stato fortunato!\n");
          MsgData msg;
	  msg.type = MSG_TYPE;
	  strcpy(msg.gadget, "Niente");
	  msg.gadget[MAX_STRING_LENGTH - 1] = '\0'; // Assicura che la stringa sia terminata correttamente
	  if (msgsnd(msgid, &msg, sizeof(MsgData) - sizeof(long), 0) == -1)
	  {
           perror("Errore nell'invio del messaggio dalla funzione");
          }
          
	  kill(getpid(), SIGTERM);
	  exit(EXIT_SUCCESS);
	}

        if (numero_random % 2 == 0) 
        {
          kill(getpid(), SIGUSR1);
        } 
        else
        {
          kill(getpid(), SIGUSR2);
        }

        pause();
    }

    return 0;
}

void f_bracciale(int s) 
{

   //dichiarazione di variabile sembuf per descrivere l'operazione del semaforo
    struct sembuf oper;
    oper.sem_num = 0;
    oper.sem_op = -1; // Decrementa il semaforo
    oper.sem_flg = SEM_UNDO;// SEM_UNDO ci garantisce che l'operazione sul semaforo venga annullata automaticamente se il processo termina in modo anomalo

    if (semop(semid, &oper, 1) == -1) 
    {
        perror("Errore nell'operazione sul semaforo");
        exit(1);
    }

    printf("Complimenti! Hai vinto un braccialetto.\n");
    
    //invio della vincita, tramite coda di messaggio, al server
    MsgData msg;
    msg.type = MSG_TYPE;
    strcpy(msg.gadget, "Bracciale");
    msg.gadget[MAX_STRING_LENGTH - 1] = '\0'; // Assicura che la stringa sia terminata correttamente
      
    if (msgsnd(msgid, &msg, sizeof(MsgData) - sizeof(long), 0) == -1)
    {
      perror("Errore nell'invio del messaggio dalla funzione f");
      return; 
    } 
    else
        printf("Messaggio inviato con successo al server.\n"); //stampa di debug per verificare l'invio corretto del messaggio
        exit(EXIT_SUCCESS);

    oper.sem_op = 1; // Incrementa il semaforo
    
    //controllo dell'operazione
    if (semop(semid, &oper, 1) == -1)
    {
       perror("Errore nell'operazione sul semaforo");
       exit(1);
    }

    kill(getpid(), SIGTERM);
    exit(0);
}

void f_collana(int s)
{
    struct sembuf oper;
    oper.sem_num = 0;
    oper.sem_op = -1; // Decrementa il semaforo
    oper.sem_flg = SEM_UNDO;

    if (semop(semid, &oper, 1) == -1) {
        perror("Errore nell'operazione sul semaforo");
        exit(1);
    }

    printf("Complimenti! Hai vinto una collanina.\n");
    
    MsgData msg;
    msg.type = MSG_TYPE;
    strcpy(msg.gadget, "Collana");
    
    msg.gadget[MAX_STRING_LENGTH - 1] = '\0'; // Assicura che la stringa sia terminata correttamente
    if (msgsnd(msgid, &msg, sizeof(MsgData) - sizeof(long), 0) == -1)
    {
     perror("Errore nell'invio del messaggio dalla funzione f");
     return; 
    }
    else
     printf("Messaggio inviato con successo al server.\n"); //stampa di debug
       
     exit(EXIT_SUCCESS);

    oper.sem_op = 1; // Incrementa il semaforo
    if (semop(semid, &oper, 1) == -1) {
        perror("Errore nell'operazione sul semaforo");
        exit(1);
    }

    kill(getpid(), SIGTERM);
    exit(0);
}

void f_stop(int s)
{

    // Rimozione del semaforo alla fine del programma
    union semun su;
    if (semctl(semid, 0, IPC_RMID, su) == -1)
    {
        perror("Errore nella rimozione del semaforo");
        exit(1);
    }

    exit(0);
}



