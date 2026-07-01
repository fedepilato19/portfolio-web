
Sistema di Parcheggio Assistito in Logica Fuzzy con Simulink e TrueTime
Questo progetto presenta la modellazione e la simulazione di un sistema di controllo in tempo reale per un parcheggio assistito automobilistico. L'obiettivo è calcolare la Forza di Intervento Totale per aiutare il veicolo nelle manovre di parcheggio, basandosi su parametri ambientali e di posizionamento rilevati dai sensori.
Componenti e Struttura del Progetto
Il sistema sfrutta un'architettura di rete wireless simulata per la trasmissione dei dati dai sensori a un nodo Gateway, che instrada poi i segnali al Controllore principale. I file inclusi tracciano l'intero ecosistema di controllo:
parcheggioAssistito_Simu.slx
Il modello grafico principale sviluppato in ambiente Simulink che gestisce la simulazione temporale e l'interconnessione tra i nodi TrueTime.
parcheggioAssistito.fis
Il file di configurazione del sistema di inferenza fuzzy basato su logica Mamdani. Definisce le funzioni di appartenenza per tre ingressi (velocità, distanza, angolo) e per l'output principale.
fuzzy_controller.m
La funzione MATLAB che si occupa di caricare il file fuzzy ed eseguire l'inferenza effettiva sui dati normalizzati inseriti.
gateway_init.m e gateway_fnctn.m
Inizializzazione e logica del nodo Gateway. Gestisce le mailbox di ricezione per i segnali dei sensori, traccia le metriche sui pacchetti persi e inoltra i segnali verso il controllore calcolando i tempi di ritardo.
controllore_init.m e controllore_fnctn.m
Inizializzazione e ciclo di esecuzione del kernel del Controllore. Estrae i messaggi di velocità, distanza e angolo dalle rispettive caselle postali e chiama il controller fuzzy per stabilire l'azione corretta.
nethandgat_fnctn.m e nethandsens_fnctn.m
I gestori di rete per i nodi, responsabili dell'estrazione dei messaggi dal network TrueTime e della generazione dinamica dei task di risposta.
Logica di Controllo Fuzzy
Il sistema analizza tre variabili in ingresso:
Velocità del veicolo: suddivisa in bassa, media e alta.
Distanza dall'ostacolo: suddivisa in vicino, medio e lontano.
Angolo di sterzata: suddiviso in piccolo, medio e grande.
L'output calcola un valore continuo che rappresenta l'intensità dell'intervento automatico del sistema per evitare collisioni e garantire un parcheggio sicuro.