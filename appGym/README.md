# Gym Project
App Gym è un'applicazione web full-stack progettata per semplificare la gestione quotidiana di una palestra. L'obiettivo del progetto è offrire una doppia interfaccia: una dashboard amministrativa per la segreteria/gestore (per controllare gli iscritti e gestire i corsi) e un'interfaccia utente fluida per i clienti della palestra

**Funzionalità principali:**
Gestione utenti e iscritti:
- Registrazione e anagrafica: Schermata dedicata all'inserimento di nuovi soci con tracciamento dei dati personali.
- Monitoraggio Abbonamenti: Controllo immediato dello stato dei pagamenti e delle scadenze delle iscrizioni per ogni utente.

**Gestione corsi**
- Calendario Dinamico: Visualizzazione e organizzazione dei corsi settimanali (es. Sala Pesi, Crossfit, Yoga, Pilates).
- Pianificazione Orari: Possibilità per l'amministratore di inserire, modificare o cancellare le sessioni di allenamento, assegnando istruttori e sale.

**Stack tecnologico**
Il progetto dimostra solide competenze nell'integrazione tra frontend e persistenza dei dati:
- Frontend: HTML5, CSS3 (con layout responsive per l'utilizzo su tablet e smartphone in sala) e JavaScript (Vanilla).
- Logica Dinamica: Uso intensivo di JavaScript asincrono (Fetch API) per aggiornare la dashboard e i corsi in tempo reale senza ricaricare la pagina.
- Database: MySQL per la memorizzazione sicura e strutturata dei dati di utenti, corsi e prenotazioni (con relativi dump .sql inclusi nel repository).

**Struttura dei file**
- palestra.html / corso.html: Le interfacce utente principali per consultare il palinsesto e iscriversi alle sessioni.
- gestione_utenti.html: La dashboard riservata allo staff per il monitoraggio dei tesserati.
- database_palestra.sql: Il file di configurazione MySQL contenente la struttura delle tabelle relazionali e i dati di test per mettere subito in funzione l'applicazione.
