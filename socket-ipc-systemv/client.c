#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>

#define PORTA 8080
#define DIM_BUFFER 1024

int socketCliente;

void* inviaMessaggio(void* arg)
{
    int socketCliente = *((int*)arg);
    char buffer[DIM_BUFFER];
    int scelta, status, opzione1, opzione2;
    int quantita;
        
    int contatoreScelta1 = 0; //un contatore che ci permette di tener conto delle volte che si seleziona opzione 1 (exec)
    int maxScelta1 = 1;// Numero massimo di volte in cui è possibile scegliere l'opzione 1
    
    do
    {
        printf("\nVuoi acquistare dal catalogo o tentare di vincere un gioiello a sorpresa?\n");
        printf("1. tentare di vincere un gioiello\n");
        printf("2. Dal catalogo\n");
        printf("0. per terminare gli acquisti\n");
        printf("\nScelta: ");
        scanf("%d", &scelta);

        if (scelta == 1) 
        {
            if (contatoreScelta1 < maxScelta1)
            {
                contatoreScelta1++;
                send(socketCliente, "Opzione1", strlen("Opzione1"), 0);// Invia un messaggio al server quando l'utente preme 1
            } 
            else
            {
               printf("Hai già raggiunto il limite massimo di scelte per l'opzione 1.\n");
               continue;
            }
        }


        if (scelta == 1 && scelta != 0 || scelta == 2 && scelta != 0)
        {
            pid_t pid = fork();
            
            switch (pid) 
            {
                case -1:
                
                    perror("Errore nella fork\n");
                    exit(EXIT_FAILURE);
                    
                case 0:
                
                    if (scelta == 1) 
                    {
                        execl("vincitasorpresa", "vincitasorpresa", NULL); //esegue il programma per tentare di vincere un gioiello
                        perror("Errore durante l'esecuzione di execl");
                        exit(EXIT_FAILURE);
                
                    } 
                    else if (scelta == 2) 
                    {
                        // Ricevi e stampa la risposta dal server
                        recv(socketCliente, buffer, sizeof(buffer), 0);
                        printf("%s\n", buffer);
                        // Pulizia del buffer di input
                        while (getchar() != '\n');
                        
                        // Inserimento del nome del prodotto desiderato
                        fgets(buffer, sizeof(buffer), stdout);
                        printf("Inserisci il nome del prodotto desiderato: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        buffer[strcspn(buffer, "\n")] = '\0';

                        printf("Il cliente ha inserito il prodotto: %s\n", buffer);

                        // Invio del nome del prodotto al server
                        send(socketCliente, buffer, strlen(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));

		        // Gestisce il caso di errore nel caso di nome non valido
		        if (strcmp(buffer, "Errore: Prodotto non valido") == 0) 
		        {
		            close(socketCliente);
		            pthread_exit(NULL);
		        }

		        // Inserimento della quantità desiderata
		        printf("Inserisci la quantità desiderata: ");
		        scanf("%d", &quantita);

		        // Pulizia del buffer di input
		        while (getchar() != '\n');

		        // Invio della quantità al server
		        snprintf(buffer, sizeof(buffer), "%d", quantita);
		        send(socketCliente, buffer, strlen(buffer), 0);
		    
		       // Gestione del caso di errore nel caso di quantità non valida
		       if (strcmp(buffer, "Errore: Prodotto non disponibile o quantità non valida\n") == 0) 
		       {
		           close(socketCliente);
		           pthread_exit(NULL);
		           memset(buffer, 0, sizeof(buffer));
		       }
		   
		       // Ricevi e stampa la risposta dal server
		       recv(socketCliente, buffer, sizeof(buffer), 0);
		       printf("%s\n", buffer);
                   
                       memset(buffer, 0, sizeof(buffer));
		    
		      // Invia un messaggio al server per indicare che vuoi ancora comprare
		      send(socketCliente, "Ancora", strlen("Ancora"), 0);
		      memset(buffer, 0, sizeof(buffer));
		

		    }
		    
		    exit(EXIT_SUCCESS);
		    
	     default:
		   wait(NULL);                         
          }
       }

    }while (scelta != 0);
    
//non appena l'utente preme 0 termina gli acquisti
    send(socketCliente, "Exit", strlen("Exit"), 0);
    close(socketCliente);
    pthread_exit(NULL);
}

int main()
{
    struct sockaddr_in indirizzoServer;

    //Creazione Socket
    if ((socketCliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Creazione socket fallita");
        exit(EXIT_FAILURE);
    }

    indirizzoServer.sin_family = AF_INET;
    indirizzoServer.sin_port = htons(PORTA);

    if (inet_pton(AF_INET, "127.0.0.1", &indirizzoServer.sin_addr) <= 0) {
        perror("Indirizzo non valido/Indirizzo non supportato");
        exit(EXIT_FAILURE);
    }


    if (connect(socketCliente, (struct sockaddr*)&indirizzoServer, sizeof(indirizzoServer)) < 0) {
        perror("Connessione fallita");
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] Connessione in corso...\n");

    pthread_t idThread;

    if (pthread_create(&idThread, NULL, inviaMessaggio, (void*)&socketCliente) < 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(idThread, NULL);

    return 0;
}






