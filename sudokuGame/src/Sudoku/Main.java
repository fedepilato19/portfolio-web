package Sudoku;
import java.io.IOException;
import java.time.Duration;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.List;
import java.util.Scanner;

/**
 * Classe principale che gestisce l'interazione utente per il gioco Sudoku.
 */

public class Main {
    public static void main(String[] args) {
        SudokuGameImpl gioco = new SudokuGameImpl(); // Creazione di un nuovo gioco Sudoku
        Scanner scanner = new Scanner(System.in); // Creazione di uno scanner per l'input dell'utente
        List<Mossa> CronologiaMosse = new ArrayList<>(); // Lista per memorizzare la cronologia delle mosse effettuate
        SudokuTimer timer = new SudokuTimer(); //Timer per il calcolo del tempo trascorso durante il gioco
        boolean salva = false;
        boolean giocoCompletato = false; // Flag per indicare se il gioco è completato

        // Loop principale del gioco
        while (!giocoCompletato) {
            // Visualizzazione del menu principale
            System.out.println("Menu principale:");
            System.out.println("1) Inizia nuova partita");
            System.out.println("2) Carica partita");
            System.out.println("3) Esci");

            int scelta;
            try {
                scelta = scanner.nextInt();
            } catch (InputMismatchException e) {
                System.out.println("Input non valido. Inserisci un numero.");
                scanner.next(); // Pulisce l'input non valido
                continue;
            }

            if (scelta < 1 || scelta > 3) {
                System.out.println("Scelta non valida. Riprova.");
                continue;
            }


            switch (scelta) {
                case 1:
                    boolean corretto = false; //flag per gestire un loop di inserimento del livello in caso di input non valido
                    while(!corretto) {
                        try {
                            System.out.println("Scegli livello di difficoltà (1 = facile, 2 = medio, 3 = difficile):");
                            int livello = scanner.nextInt();
                            gioco.startGame(livello); // Inizia una nuova partita con il livello selezionato
                            //se non sono stati catturati errori, il gioco comincia e il flag viene impostato true per uscire dal loop di inserimento del livello
                            corretto = true;
                            CronologiaMosse.clear();  // Pulisce la cronologia delle mosse
                            timer.start();  // Avvia il timer
                        } catch (InputMismatchException e) {
                            // Se l'utente ha inserito una stringa che non può essere convertita in un numero
                            System.out.println("Input non valido. Devi inserire un numero.");
                            scanner.next(); // Pulisce l'input non valido
                        }
                         catch (IllegalArgumentException e) {
                            System.out.println(e.getMessage());
                        }
                    }
                    break;
                case 2:
                    System.out.println("Inserisci il nome del file da caricare:");
                    String NomeFile = scanner.next();
                    try {
                        gioco = SudokuGameImpl.CaricaPartita(NomeFile); // Carica una partita
                        gioco.displayGrid();
                        CronologiaMosse.clear(); // Pulisce la cronologia delle mosse
                        timer.setTempoInizio(gioco.TempoTrascorso()); // Imposta il tempo iniziale che aveva la partita caricata
                        timer.start();// Avvia il nuovo timer
                    } catch (IOException | ClassNotFoundException e) {
                        System.out.println("Errore nel caricamento del file.");
                        e.printStackTrace();
                    }
                    break;
                case 3:
                    System.out.println("Grazie per aver giocato. Arrivederci!");
                    System.exit(0);
            }

            // Loop per il gioco attivo
            while (!gioco.isGameComplete()) {
                System.out.println("Inserisci mossa (riga colonna valore, da 1 a 9), 'undo' per annullare l'ultima mossa, 'salva' per salvare la partita e 'esci' per uscire:");
                String input = scanner.next();

                // Gestione del comando 'undo' per annullare l'ultima mossa
                if (input.equalsIgnoreCase("undo")) {
                    if (!CronologiaMosse.isEmpty()) {
                        Mossa UltimaMossa = CronologiaMosse.removeLast();
                        gioco.griglia.setCella(UltimaMossa.riga, UltimaMossa.col, UltimaMossa.precVal);
                        gioco.displayGrid();
                        System.out.println("Ultima mossa annullata.");
                    } else {
                        System.out.println("Non ci sono mosse da annullare.");
                    }
                    continue;
                }


                // Gestione del comando 'salva' per salvare la partita
                if (input.equalsIgnoreCase("salva")) {
                    System.out.println("Inserisci il nome del file:");
                    String salvaFile = scanner.next();
                    try {
                        Duration elapsedTime = timer.stampaProvTempotot(); //salva il tempo totale trascorso fino al salvamento
                        gioco.SalvaPartita(salvaFile, elapsedTime); //salva la partita
                        System.out.println("Partita salvata con successo. Tempo trascorso: " + timer.Tempotot()); // stampa il tempo totale trascorso dall'inizio della partita al salvataggio
                        salva = true;
                    } catch (IOException e) {
                        System.out.println("Errore nel salvataggio del file.");
                        e.printStackTrace();
                    }
                    continue;
                }


                // Gestione del comando 'esci' per uscire dal gioco
                if (input.equalsIgnoreCase("esci")) {

                    if(!salva) {
                        System.out.println("Vuoi salvare la partita prima di uscire?(scrivi 's' per salvare o un qualsiasi input per non farlo)");
                        String scelta1 = scanner.next();
                        if (scelta1.equalsIgnoreCase("s")) {
                            System.out.println("Inserisci il nome del file:");
                            String salvaFile = scanner.next();
                            try {
                                Duration elapsedTime = timer.stampaProvTempotot();
                                gioco.SalvaPartita(salvaFile, elapsedTime);
                                System.out.println("Partita salvata con successo. Tempo trascorso: " + timer.Tempotot());
                            } catch (IOException e) {
                                System.out.println("Errore nel salvataggio del file.");
                                e.printStackTrace();
                            }
                        }
                    }
                    System.out.println("Grazie per aver giocato. Arrivederci!");
                    System.exit(0);
                }


                int riga, col, val;

                try {
                    riga = Integer.parseInt(input) - 1; // Converti da 1-9 a 0-8
                    col = scanner.nextInt() - 1; // Converti da 1-9 a 0-8
                    val = scanner.nextInt();

                    // Salva lo stato corrente della cella
                    int precVal = gioco.griglia.Cella(riga, col);
                    //verifica la validità della mossa e nel caso setta la cella
                    gioco.makeMove(riga, col, val);
                    // Aggiunge la mossa alla cronologia solo dopo aver verificato la validità della mossa
                    CronologiaMosse.add(new Mossa(riga, col, precVal));
                    gioco.displayGrid();// Visualizza la griglia dopo la mossa effettuata


                } catch (InputMismatchException | IllegalArgumentException e) {
                    System.out.println("Errore: " + e.getMessage());
                    scanner.nextLine();
                }
            }

            if (gioco.isGameComplete()) {
                timer.stop();// Ferma il timer quando il gioco è completato
                String elapsedTime = timer.Tempotot(); // Ottiene il tempo trascorso
                System.out.println("Congratulazioni! Hai completato il Sudoku in " + elapsedTime + "!");

                //ti permette di salvare la partita anche una volta completata
                System.out.println("Vuoi salvare la partita? (scrivi 's' per salvare o un qualsiasi input per non farlo)");
                String scelta2 = scanner.next();
                if (scelta2.equalsIgnoreCase("s")) {
                    System.out.println("Inserisci il nome del file:");
                    String salvaFile = scanner.next();
                    try {
                        gioco.SalvaPartita(salvaFile, timer.stampaProvTempotot());
                        System.out.println("Partita salvata con successo. Tempo trascorso: " + elapsedTime);
                    } catch (IOException e) {
                        System.out.println("Errore nel salvataggio del file.");
                        e.printStackTrace();
                    }
                }

                    System.out.println("Vuoi continuare a giocare o vuoi uscire? (Scrivi 'continua' per continuare a giocare o un qualsiasi input per uscire)");
                    String scelta3 = scanner.next();
                    if (!scelta3.equalsIgnoreCase("continua")) {
                        System.out.println("Grazie, alla prossima!");
                        giocoCompletato = true;
                    } else {
                        CronologiaMosse.clear();
                    }
            }
        }
    }
}