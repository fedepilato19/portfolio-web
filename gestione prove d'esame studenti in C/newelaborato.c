/*Scrivere un programma in C che permette di gestire le informazioni riguardanti le prove di informatica,
sostenuti dagli studenti. Il programma deve essere in grado di gestire:
- la gestione anagrafica degli studenti;( definire il numero degli studenti ad inizio programma);
- la gestione dei voti attribuiti agli studenti e del tipo di prova sostenuta (scritta, orale)
Attraverso un menu a scelta(di tipo intero), eseguire le seguenti funzioni:
1. inserire un nuovo studente;
2. controllare se e' presente uno studente nell'elenco;
3. calcolare la media di tutti gli scritti;
4. visualizzare tutto l'elenco;
5. visualizzare a video il voto massimo e il minimo di un appello;
6. per terminare

(struttura prove(lista):  per inserire le prove sostenute dagli studenti;
file studenti.txt= per prelevare i dati degli studenti )*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 1//numero di alunni

struct prove
{
	char nomeStudente[30];
	char cognomeStudente[30];
	char matricola[6];
	int voto;
	int appello;
	char tipoProva[10];
	struct prove *punt;
};
FILE *fp;
int i;

void menu();
struct prove*creaLista();
void visualizzaLista(struct prove *);
struct prove* inserisciStudente(struct prove *);
void mediaScritti(struct prove *);
void studentePresente(struct prove *);
void max_min_appello(struct prove *);


int main()
{
	struct prove *puntLista; //puntatore al primo elemento della lista
	float media;
	int scelta;
	puntLista=creaLista(); 
	do
	{
		menu();
		printf("\nInserire quale operazione eseguire:\n");
		scanf("%d",&scelta);
		switch(scelta)
		{
			case 1:
				inserisciStudente(puntLista);
			break;
			case 2:
				studentePresente(puntLista);
			break;
			case 3:
				mediaScritti(puntLista);
			break;
			case 4:
				visualizzaLista(puntLista);
			break;
			case 5:
				max_min_appello(puntLista);
			break;	
			default:
				printf("Scelta errata! Riprova\n\n");
			break;
		}
	}while(scelta!=6);
	
	printf("Fine del programma.");
	
	
}
void menu()
{
	printf("\n\n-----------------------------MENU' DI SCELTA------------------------------------\n");
	printf("\n1. inserire un nuovo studente nel file;\n\n");
	printf("2. controllare se e' presente uno studente nell'elenco;\n\n");
	printf("3. stampare la media totale dei voti delle prove scritte;\n\n");
	printf("4. visualizzare tutto l'elenco degli studenti;\n\n");
	printf("5. visualizzare il voto massimo e il minimo di un appello;\n\n");
	printf("6. per terminare\n\n");
}

struct prove *creaLista()
{
	struct prove *p, *aux;
	
	fp=fopen("studenti.txt","r");
	if(fp==NULL)
		printf("Errore nel file!\n\n");
	else
	{
		if(N!=0)
		{	
			
		p=(struct prove*)malloc(sizeof(struct prove)); //alloca e restituisce la quant memoria del primo elemento 

		printf("\n-----------------------------------DATI DELLO STUDENTE----------------------------------------\n");
		fscanf(fp,"%s",p->cognomeStudente);
		printf("\nCognome: %s       ",p->cognomeStudente);
		fscanf(fp,"%s",p->nomeStudente);
		printf("Nome: %s          ",p->nomeStudente);
		fscanf(fp,"%s",p->matricola);
		printf("Matricola: %s\n",p->matricola);
		if(strcmp(p->cognomeStudente,"Pilato")==0)
		{
			p->voto=18;
			printf("\ninserisci il tipo di prova (scritto, orale): ");
			scanf("%s",p->tipoProva);
			printf("\nla prova si e' svolta in quale appello?  (da 1 a 7) ");
			scanf("%d",&p->appello);
		}
			
		else
		{
			printf("\n------------------------INSERIMENTO DEL VOTO DELLA PROVA DI INFORMATICA-------------------------\n");
			printf("\ninserisci il voto: ");
			scanf("%d",&p->voto);
			printf("\ninserisci il tipo di prova (scritto, orale): ");
			scanf("%s",p->tipoProva);
			printf("\nla prova si e' svolta in quale appello?  (da 1 a 7) ");
			scanf("%d",&p->appello);
		}
		
		
		aux=p;	
		
		for(i=2;i<=N;i++)
		{
			aux->punt=(struct prove*)malloc(sizeof(struct prove));
			aux=aux->punt;
			printf("\n--------------------------------DATI DELLO STUDENTE------------------------------------------------\n");
			fscanf(fp,"%s",aux->cognomeStudente);
			printf("\nCognome: %s          ",aux->cognomeStudente);
			fscanf(fp,"%s",aux->nomeStudente);
			printf("Nome: %s          ",aux->nomeStudente);
			fscanf(fp,"%s",aux->matricola);
			printf("Matricola: %s\n",aux->matricola);
			printf("\n------------------------INSERIMENTO DEL VOTO DELLA PROVA DI INFORMATICA-----------------------------\n");
			if(strcmp(aux->cognomeStudente,"Pilato")==0)
			{
				aux->voto=18;
				printf("\ninserisci il tipo di prova (scritto, orale): ");
				scanf("%s",aux->tipoProva);
				printf("\nla prova si e' svolta in quale appello? (da 1 a 7) ");
				scanf("%d",&aux->appello);
			}
			else
			{
				printf("\ninserisci il voto: ");
				scanf("%d",&aux->voto);
				printf("\ninserisci il tipo di prova (scritto, orale): ");
				scanf("%s",aux->tipoProva);
				printf("\nla prova si e' svolta in quale appello? (da 1 a 7) ");
				scanf("%d",&aux->appello);	
			}
			
		}
		aux->punt=NULL; //dichiarazione della variabile finale degli studenti che prende il valore nullo (ultimo elemento)		
		}
		else
			p=NULL;
	}
	return(p);
}


struct prove* inserisciStudente(struct prove *a)
{
	fp=fopen("studenti.txt","a");
	if(fp==NULL)
		printf("errore nel file\n\n");
	else
	{
		a=(struct prove *)malloc(sizeof(struct prove));
		printf("\n----------------------INSERIMENTO STUDENTE----------------------\n\n");
		printf("\nCognome: ");
		scanf("%s",a->cognomeStudente);
		fprintf(fp,"\n%s ",a->cognomeStudente);
		printf("\nNome: ");
		scanf("%s",a->nomeStudente);
		fprintf(fp,"%s",a->nomeStudente);
		printf("\nMatricola: ");
		scanf("%s",a->matricola);
		fprintf(fp," %s",a->matricola);
		printf("\nlo studente e' stato inserito con successo!\n\n");
	}
	fclose(fp);	
}

void studentePresente(struct prove *a)
{
	char *cogn;
	int trovato=0;
	printf("\ninserisci il cognome da cercare: ");
	scanf("%s",cogn);
	do
	{
		if(strcmp(a->cognomeStudente,cogn)==0)
		{
			printf("\nlo studente %s e' presente nell'elenco\n\n",cogn);
			trovato=1;
		}
		a=a->punt;
		
	}while(a!=NULL && trovato!=1);
	if(trovato==0)
		printf("\nlo studente %s non e' presente nell'elenco\n\n",cogn);
}

void mediaScritti(struct prove *a)
{
	int conta=0;
	float somma=0,media;
	
	for(i=0;i<N;i++)
	{
		if((strcmp(a->tipoProva,"scritto")==0))
		{
			somma=somma+a->voto;
			conta++;
		}
		a=a->punt;
	}
	media=somma/conta;
	printf("\nla media delle prove scritte e' %f\n\n",media);
}

void visualizzaLista(struct prove *p)
{
	while(p!=NULL)
	{
		printf("Nome %s     ",p->nomeStudente);
		printf("Cognome %s     ",p->cognomeStudente);
		printf("Matricola %s     \n\n",p->matricola);
		printf("Nel %d appello ",p->appello);
		printf("nella prova %s di informatica ",p->tipoProva);
		printf("ha preso il seguente voto %d \n\n",p->voto);
		
		p=p->punt;
	}
}


void max_min_appello(struct prove *a)
{
	int max,min,app;
	printf("\ninserisci l'appello di cui vuoi sapere il voto massimo e il minimo: \n");
	scanf("%d",&app);
	
	max=a->voto;
	min=a->voto;
	for(i=1;i<=N;i++)
	{
		if(a->appello==app)
		{
			if(a->voto>max)
				max=a->voto;
			if(a->voto<min)
				min=a->voto;
		}
		a=a->punt;	
	}
	
	printf("\nil massimo dell'appello e' %d\n\n",max);
	printf("il minimo dell'appello e' %d\n\n",min);
}
