### Gioco del Sudoku Interattivo in Linguaggio Java
Questo progetto implementa un'applicazione completa e orientata agli oggetti per il gioco del Sudoku in linguaggio Java. Il programma consente di giocare interamente da terminale testuale, offrendo diversi livelli di difficoltà, il tracciamento del tempo di gioco e la possibilità di sospendere e riprendere le partite in un secondo momento.
##Struttura dei File nel Progetto
- **Main.java** 
La classe principale che gestisce il ciclo di vita dell'applicazione, l'interazione diretta con l'utente tramite menu e l'esecuzione coordinata di tutte le componenti di gioco.
- **SudokuGame.java**
L'interfaccia principale che definisce il contratto comportamentale del gioco, descrivendo i metodi essenziali per l'avvio, l'esecuzione di una mossa e la verifica dello stato della griglia.
- **SudokuGameImpl.java**
L'implementazione concreta dell'interfaccia SudokuGame. Gestisce la logica di gioco e integra i flussi di Input/Output per implementare il salvataggio e il caricamento dello stato.
- **SudokuGrid.java e SudokuGrid.class**
La classe responsabile della struttura dati della griglia di gioco (matrice 9x9), del controllo dei vincoli di riga, colonna e sotto-griglia 3x3 e della validità di ogni mossa inserita.
- **SudokuSolver.java**
Il modulo algoritmico che si occupa di risolvere automaticamente la griglia corrente utilizzando una tecnica di esplorazione esaustiva con backtracking, utile anche a generare schemi iniziali validi.
- **Mossa.java e Mossa.class**
Una classe di supporto che modella la singola mossa effettuata dal giocatore, tenendo traccia delle coordinate di riga, colonna e del valore precedente per scopi di storico.
- **SudokuTimer.java**
Un componente per la misurazione dinamica del tempo trascorso durante la sessione di gioco attiva, in grado di cumulare i secondi anche dopo il caricamento di un salvataggio precedente.
I file .class associati rappresentano i bytecode compilati pronti per essere eseguiti dalla Java Virtual Machine.
### Funzionalità Principali e Tecnologie Applicate
- **Programmazione Orientata agli Oggetti (OOP)**: Separazione netta delle responsabilità tramite interfacce, classi di modello, logiche di controllo e moduli algoritmici isolati.
- **Persistenza dei Dati (Serializzazione degli Oggetti)**: Utilizzo delle interfacce ObjectOutputStream e ObjectInputStream per serializzare e salvare su file l'intero stato dell'oggetto di gioco, preservando la griglia e il timer per un ripristino futuro.
- **Algoritmo di Backtracking**: Implementazione di un motore ricorsivo di risoluzione all'interno di SudokuSolver che analizza le combinazioni valide per verificare che lo schema generato sia completabile.
- **Robustezza del Codice (Exception Handling)**: Gestione degli errori di input dell'utente tramite blocchi try-catch, prevenendo arresti anomali in caso di inserimento di caratteri non numerici (InputMismatchException) o parametri non consentiti.
