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

#define PORTA 8080
#define MAX_CLIENTI 10
#define MAX_PRODOTTI 100
#define LUNGHEZZA_TIPO_PRODOTTO_MAX 20
#define MSG_KEY 12345
#define MSG_TYPE 1
#define MAX_STRING_LENGTH 100

int socketServer, socketCliente;
int msgid;

// Dichiarazione della variabile globale per indicare la terminazione
volatile int terminaProgramma = 0;

//struttura che specifica i dati del messaggio che riceve dal programma eseguito nel client 'vincitasorpresa'
typedef struct {
    long type;
    char gadget[MAX_STRING_LENGTH];
} MsgData;

//struttura che specifica i dati dei prodotti
typedef struct {
    char tipo[LUNGHEZZA_TIPO_PRODOTTO_MAX];
    int quantita;
    int Prezzi;
} Prodotto;


typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t disponibile[MAX_PRODOTTI]; // Variabili di condizione per ogni prodotto
    Prodotto inventario[MAX_PRODOTTI];
    int numTipiProdotti;
} Inventario;

Inventario *memoriaCondivisa;  // Puntatore alla memoria condivisa
sem_t *semv;

// Gestione del segnale di terminazione
void gestisciSegnaleTerminazione(int sig) {
   
     printf("Ricevuto segnale di terminazione. Chiudo il server...\n");
    
  
     // Imposto la variabile di stato per indicare ai thread di terminare
    terminaProgramma = 1;
    
    // Libero la memoria condivisa
    if (shmdt(memoriaCondivisa) == -1) {
        perror("Errore nella rimozione della memoria condivisa");
        exit(EXIT_FAILURE);
    }
    
    // Rimuovo il semaforo
    if (sem_close(semv) == -1) {
        perror("Errore nella chiusura del semaforo POSIX");
        exit(EXIT_FAILURE);
    }
    
    // Elimino il semaforo
    if (sem_unlink("sema") == -1) {
        perror("Errore nella rimozione del semaforo POSIX");
        exit(EXIT_FAILURE);
    }
  
   // Rimuovo la coda di messaggi
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("Errore nella rimozione della coda di messaggi");
        exit(EXIT_FAILURE);
    }

    // Esce dal programma
    exit(EXIT_SUCCESS);

}


void inizializzaInventario(Inventario *memoriaCondivisa) {
    pthread_mutex_init(&memoriaCondivisa->mutex, NULL);//inizializzazione mutex

    const char *tipiProdotti[] = {"Bracciale", "Collana", "Orecchini", "Ciondoli"};
    int prezzi[] = {15, 20, 10, 8};
    memoriaCondivisa->numTipiProdotti = sizeof(tipiProdotti) / sizeof(tipiProdotti[0]);

    pthread_mutex_lock(&memoriaCondivisa->mutex); //blocca l'accesso concorrente ad altre operazioni che coinvolgono la memoria condivisa.

    for (int i = 0; i < memoriaCondivisa->numTipiProdotti; ++i) 
    {
        pthread_cond_init(&memoriaCondivisa->disponibile[i], NULL);//variabile di condizione che viene utilizzata per la sincronizzazione tra i thread quando i prodotti sono disponibili
    
        strncpy(memoriaCondivisa->inventario[i].tipo, tipiProdotti[i], sizeof(memoriaCondivisa->inventario[i].tipo) - 1);
        memoriaCondivisa->inventario[i].tipo[sizeof(memoriaCondivisa->inventario[i].tipo) - 1] = '\0';
        memoriaCondivisa->inventario[i].quantita = 10;
        memoriaCondivisa->inventario[i].Prezzi = prezzi[i];
    }

    pthread_mutex_unlock(&memoriaCondivisa->mutex);//rilascio del mutex
}

void inizializzaSharedMemory()
{
    int shmid;

    // Creazione della memoria condivisa
    if ((shmid = shmget(IPC_PRIVATE, sizeof(Inventario), IPC_CREAT | 0666)) < 0) {
        perror("Errore nella creazione della memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // Collegamento della memoria condivisa al processo
    if ((memoriaCondivisa = (Inventario *)shmat(shmid, NULL, 0)) == (Inventario *)-1) {
        perror("Errore nel collegamento della memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // Inizializzazione dell'inventario
    inizializzaInventario(memoriaCondivisa);
}

void* gestisciClient(void* arg)
{
  while (!terminaProgramma) 
 {
    int *socketClientePtr = (int*)arg;
    int socketCliente = *socketClientePtr;
    free(arg);

    char buffer[1024] = {0};
    char nomeProdotto[LUNGHEZZA_TIPO_PRODOTTO_MAX];
    int quantitaRichiesta;
    

    send(socketCliente, buffer, strlen(buffer), 0);

    while (1)
    {
    
        pthread_mutex_lock(&memoriaCondivisa->mutex);
        
        //invio del catalogo
        for (int i = 0; i < memoriaCondivisa->numTipiProdotti; ++i)
        {
          snprintf(buffer, sizeof(buffer), "%s (%d disponibili), (€%d 1 pz.)\n", memoriaCondivisa->inventario[i].tipo, memoriaCondivisa->inventario[i].quantita, memoriaCondivisa->inventario[i].Prezzi);
          send(socketCliente, buffer, strlen(buffer), 0);
          memset(buffer, 0, sizeof(buffer));
        }

        pthread_mutex_unlock(&memoriaCondivisa->mutex);
        
        recv(socketCliente, nomeProdotto, sizeof(nomeProdotto), 0);
        nomeProdotto[sizeof(nomeProdotto) - 1] = '\0';

        if (strcmp(nomeProdotto, "Exit") == 0) 
        {    
            // il Cliente si è disconnesso, termina il thread
            printf("Cliente si è disconnesso, termina il thread.\n");
            close(socketCliente);
            pthread_exit(NULL);
        }
        
        // Verifica se il messaggio ricevuto è "Opzione1"
        if (strcmp(nomeProdotto, "Opzione1") == 0)
        {
            
           // Ripulisce il contenuto di nomeProdotto
           memset(nomeProdotto, 0, sizeof(nomeProdotto));

           MsgData msg;
           // Riceve un messaggio dalla coda di messaggi
           if (msgrcv(msgid, &msg, sizeof(MsgData) - sizeof(long), MSG_TYPE, 0) == -1)
           {
                perror("Errore nella ricezione del messaggio");
                exit(EXIT_FAILURE);
           }
           
            // Gestisce il messaggio ricevuto
            printf("\nMessaggio ricevuto. Il client ha estratto un numero...Vincita : %s\n", msg.gadget);

            // Continua il loop senza eseguire il resto del codice per questo ciclo
            continue;
        }
         

        sem_wait(semv);
        printf("\nIl cliente ha richiesto il prodotto: %s\n", nomeProdotto);

        size_t lunghezzaEffettivaNomeProdotto = strlen(nomeProdotto);

        recv(socketCliente, buffer, sizeof(buffer), 0);

        if (lunghezzaEffettivaNomeProdotto >= LUNGHEZZA_TIPO_PRODOTTO_MAX)
        {
            send(socketCliente, "Errore: Nome del prodotto non valido", strlen("Errore: Nome del prodotto non valido"), 0);
            continue; // Continua il loop senza chiudere il socket o terminare il thread
        }
         
         
        if (sscanf(buffer, "%d", &quantitaRichiesta) != 1)
        {
            send(socketCliente, "Errore: Quantità non valida", strlen("Errore: Quantità non valida"), 0);
            continue; // Continua il loop senza chiudere il socket o terminare il thread
        }
        else
         printf("Il cliente ha richiesto la quantità : %d\n", quantitaRichiesta);
         
        pthread_mutex_lock(&memoriaCondivisa->mutex);

        int indiceProdotto = -1;// Inizializzazione dell'indice del prodotto a -1, che indica che il prodotto non è stato trovato inizialmente
        
        // Ricerca il prodotto corrispondente al nome specificato
        for (int i = 0; i < memoriaCondivisa->numTipiProdotti; ++i)
        {
            if (strcmp(nomeProdotto, memoriaCondivisa->inventario[i].tipo) == 0)
            {
                indiceProdotto = i;  // Se il nome del prodotto corrisponde, salva l'indice del prodotto trovato
                break;
            }
        }
        
         // Se il prodotto non è disponibile, sospendi il thread
         while (indiceProdotto == -1 || memoriaCondivisa->inventario[indiceProdotto].quantita < 1)
         {
           pthread_cond_wait(&memoriaCondivisa->disponibile[indiceProdotto], &memoriaCondivisa->mutex);
         }

        if (indiceProdotto != -1)
        {
         
            // Controlla se il prodotto è disponibile
            if (memoriaCondivisa->inventario[indiceProdotto].quantita >= quantitaRichiesta && quantitaRichiesta > 0)
            {
            
            memoriaCondivisa->inventario[indiceProdotto].quantita -= quantitaRichiesta;
            printf("Il cliente ha acquistato %d unità del prodotto %s. Nuova quantità disponibile: %d\n",
                       quantitaRichiesta, memoriaCondivisa->inventario[indiceProdotto].tipo, memoriaCondivisa->inventario[indiceProdotto].quantita);
                       
           // Se il prodotto è terminato, reinseriscilo nell'inventario e notifica i clienti in attesa
            if (memoriaCondivisa->inventario[indiceProdotto].quantita == 0)
            {
            printf("\nAggiornamento inventario in corso...fatto!\n");
            memoriaCondivisa->inventario[indiceProdotto].quantita = 10; // Riaggiungi il prodotto all'inventario
            pthread_cond_broadcast(&memoriaCondivisa->disponibile[indiceProdotto]); // Notifica i clienti in attesa
            }
                 
                //invia una conferma dell'acquisto
                snprintf(buffer, sizeof(buffer), "Operazione completata con successo. Prodotto: %s, Quantità acquistata: %d\n",memoriaCondivisa->inventario[indiceProdotto].tipo, quantitaRichiesta);
                memset(nomeProdotto, 0, sizeof(nomeProdotto));
                send(socketCliente, buffer, strlen(buffer), 0);
                memset(nomeProdotto, 0, sizeof(nomeProdotto));

              

                if (strcmp(buffer, "Exit") == 0)
                {
                    // il Cliente si è disconnesso, termina il thread
                    pthread_mutex_unlock(&memoriaCondivisa->mutex);
                    close(socketCliente);
                    sem_post(semv);
                    pthread_exit(NULL);
                }
                
            }
             else
            {
                // Prodotto non disponibile o quantità non valida
                send(socketCliente, "Errore: Prodotto non disponibile o quantità non valida\n", strlen("Errore: Prodotto non disponibile o quantità non valida"), 0);
            }
            
            
         }
          else
         {
            // Prodotto non valido
            send(socketCliente, "Errore: Prodotto non valido", strlen("Errore: Prodotto non valido"), 0);
         }
        
          //Attendere il messaggio "Ancora" dal client per continuare
          memset(buffer, 0, sizeof(buffer));
          recv(socketCliente, buffer, sizeof(buffer), 0);
        
        pthread_mutex_unlock(&memoriaCondivisa->mutex);
        sem_post(semv);  
        
   }

  }
}

int main()
{
  
    struct sockaddr_in indirizzoServer, indirizzoCliente;
    socklen_t lunghezzaIndirizzo = sizeof(indirizzoCliente);

    //creazione del semaforo
    sem_unlink("sema");
    semv = sem_open("sema", O_CREAT | O_EXCL, 0666, 1);
    if (semv == SEM_FAILED)
    {
        perror("Errore semaforo posix");
        exit(EXIT_FAILURE);
    }

    pthread_t idThread;

    // Registrazione del gestore dei segnali per SIGINT
    signal(SIGINT, gestisciSegnaleTerminazione);
       
    //creazione del socket
    if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("creazione socket fallita");
        exit(EXIT_FAILURE);
    } 
    else
    {
        printf("Socket creato con successo...\n");
    }

    // Configurazione dell'indirizzo del server
    indirizzoServer.sin_family = AF_INET;
    indirizzoServer.sin_addr.s_addr = INADDR_ANY;
    indirizzoServer.sin_port = htons(PORTA);

    // Binding del socket all'indirizzo del server
    if (bind(socketServer, (struct sockaddr*)&indirizzoServer, sizeof(indirizzoServer)) < 0)
    {
        perror("binding fallito");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Binding eseguito con successo...\n");
    }

    // Il server inizia ad ascoltare per le connessioni dei client
    if (listen(socketServer, MAX_CLIENTI) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server in ascolto...\n");
    
    // Creazione della coda di messaggi
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1) 
    {
        perror("Errore nella creazione/apertura della coda di messaggi");
        exit(EXIT_FAILURE);
    }
    else
      printf("Coda di messaggi aperta correttamente nel server.\n");//stampa di debug

    inizializzaSharedMemory();

    // Accettazione delle connessioni dei client e creazione di thread per gestirle
    while (1) 
    {
        if ((socketCliente = accept(socketServer, (struct sockaddr*)&indirizzoCliente, &lunghezzaIndirizzo)) < 0) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int *socketClientePtr = malloc(sizeof(int));
        if (socketClientePtr == NULL) 
        {
            perror("malloc fallita");
            exit(EXIT_FAILURE);
        }
        
        *socketClientePtr = socketCliente;

        if (pthread_create(&idThread, NULL, gestisciClient, (void *)socketClientePtr) < 0) 
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    close(socketServer);

    return 0;
}



