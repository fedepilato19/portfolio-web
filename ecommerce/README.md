# E-Commerce Full-Stack – Piattaforma di Vendita & Gestione Articoli

Questo progetto consiste in un'applicazione web e-commerce full-stack che permette la visualizzazione, l'inserimento e la rimozione di prodotti, oltre alla gestione dinamica di un carrello della spesa. Il software include sia l'interfaccia per il cliente finale che un pannello di gestione per l'amministratore del negozio.

**Sviluppo in Team e Personalizzazione**
Il progetto è stato sviluppato in un'ottica di co-working e allineamento didattico. All'interno della repository sono presenti due varianti delle componenti chiave (Frontend e Backend) per testimoniare il processo di studio, replica e ottimizzazione del codice:
- Carrello & Server Standard (carrello.html / server.js): La base di partenza condivisa e configurata con credenziali di database specifiche.
- Carrello & Server Personalizzati (carrelloMio.html / serverMio.js): La mia implementazione autonoma, focalizzata sull'ottimizzazione dei flussi di lavoro locali (credenziali flessibili per XAMPP) e sul miglioramento delle funzioni del carrello (modifica rapida delle quantità con icone dedicate e ricalcolo automatico).

**Funzionalità principali**
- Catalogo Prodotti Dinamico (frontend.html): Visualizzazione degli articoli (Elettronica, Audio, Accessori) con caricamento dinamico delle immagini di inventario (laptop.jpeg, smartwatch.jpeg, ecc.).
- Pannello di Controllo Amministratore (Ecommerce.html): Una dashboard riservata alla gestione del magazzino per aggiungere nuovi prodotti o eliminare gli articoli esistenti in tempo reale.
- Gestione Avanzata del Carrello: Sistema interattivo per aggiungere prodotti, aggiornare il numero di pezzi desiderati (con controlli di validazione per evitare quantità inferiori a 1) e rimuovere elementi dal riepilogo d'acquisto.

**Stack tecnologico**
- Frontend: HTML5, CSS3 (stile personalizzato), JavaScript (Vanilla) con largo uso di asincronia (Fetch API) per comunicare con i servizi backend senza ricaricare la pagina.
- Backend: Node.js con framework Express per la creazione di un'architettura API RESTful pulita e performante.
- Database & Persistenza: MySQL per la modellazione relazionale dei dati tramite due tabelle dedicate (articoli e carrello), collegate tramite vincoli di Foreign Key (chiave esterna) per garantire l'integrità dei dati.
