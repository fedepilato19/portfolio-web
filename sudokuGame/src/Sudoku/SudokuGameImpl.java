package Sudoku;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serial;
import java.io.Serializable;
import java.time.Duration;

/**
 * Implementazione dell'interfaccia SudokuGame.
 * Questa classe gestisce le operazioni del gioco del Sudoku, incluse la generazione del puzzle,
 * l'esecuzione delle mosse, la verifica del completamento del gioco, e la visualizzazione della griglia.
 * Supporta inoltre il salvataggio e il caricamento dello stato del gioco.
 *
 * @version 1.0 //specifica la versione della classe o dell'interfaccia a cui è associata.
 */
public class SudokuGameImpl implements SudokuGame, Serializable {
    @Serial
    private static final long serialVersionUID = 1L;

    /** La griglia del Sudoku. */
    public final SudokuGrid griglia;

    /** Il risolutore di Sudoku. */
    private final SudokuSolver solver;

    private Duration TempoTrascorso;  //  per tracciare il tempo trascorso

    /**
     * Crea una nuova istanza del gioco del Sudoku.
     * Inizializza una griglia vuota e un risolutore di Sudoku.
     */
    public SudokuGameImpl() {
        griglia = new SudokuGrid();
        solver = new SudokuSolver(griglia);
        TempoTrascorso = Duration.ZERO;
    }

    /**
     * Restituisce il tempo trascorso durante il gioco.
     *
     * @return Il tempo trascorso
     */
    public Duration TempoTrascorso() {
        return TempoTrascorso;
    }

    /**
     * Avvia una nuova partita di Sudoku con il livello di difficoltà specificato.
     *
     * @param level Il livello di difficoltà del puzzle

     * Genera un nuovo puzzle di Sudoku con il livello di difficoltà specificato.
     */
    @Override
    public void startGame(int level) {
        if (level < 1 || level > 3) {
            throw new IllegalArgumentException("Livello di difficoltà non valido. Inserisci un numero tra 1 e 3.");
        }
        solver.Risoluzione(griglia);
        int CelleRimosse = switch (level) {
            case 1 -> 3;
            case 2 -> 20;
            default -> 35;
        };
        solver.RimuovereCelle(CelleRimosse);
        displayGrid();
    }

    /**
     * Esegue una mossa nel gioco di Sudoku specificando riga, colonna e valore.
     *
     * @param riga   La riga della cella in cui eseguire la mossa
     * @param col   La colonna della cella in cui eseguire la mossa
     * @param val Il valore da inserire nella cella
     * @throws IllegalArgumentException se la mossa non è valida
     */
    @Override
    public void makeMove(int riga, int col, int val) {

        if (!griglia.Validita_Indici(riga, col)) {
            throw new IllegalArgumentException("Indici Non Validi.");

        }else if (!griglia.MossaValida(riga, col, val)) {
            throw new IllegalArgumentException("Mossa non valida."); //non controllata

        } else{
            griglia.setCella(riga, col, val);}
 
    }

    /**
     * Verifica se il gioco di Sudoku è completo.
     *
     * @return true se il gioco è completo, altrimenti false
     */
    @Override
    public boolean isGameComplete () {
        for (int riga = 0; riga < 9; riga++) {
            for (int col = 0; col < 9; col++) {
                int val = griglia.Cella(riga, col);
                if (val == 0 || !griglia.PosizioneValida(riga, col, val)) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Visualizza la griglia di Sudoku.
     */
    @Override
    public void displayGrid() {
        System.out.println("+-------+-------+-------+");
        for (int row = 0; row < 9; row++) {
            if (row > 0 && row % 3 == 0) {
                System.out.println("|-------+-------+-------|");
            }
            for (int col = 0; col < 9; col++) {
                if (col > 0 && col % 3 == 0) {
                    System.out.print("| ");
                }
                int value = griglia.Cella(row, col);
                if (value == 0) {
                    System.out.print("  ");
                } else {
                    System.out.print(value + " ");
                }
            }
            System.out.println("|");
        }
        System.out.println("+-------+-------+-------+");
    }

    /**
     * Salva lo stato corrente del gioco in un file.
     *
     * @param NomeFile Il nome del file in cui salvare lo stato del gioco
     * @param TempoTrascorso Il tempo trascorso per la partita
     * @throws IOException se si verifica un errore durante il salvataggio del gioco
     */
    public void SalvaPartita(String NomeFile, Duration TempoTrascorso) throws IOException {
        this.TempoTrascorso = TempoTrascorso;
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(NomeFile))) {
            out.writeObject(this);
        }
    }

    /**
     * Carica uno stato salvato del gioco da un file.
     *
     * @param NomeFile Il nome del file da cui caricare lo stato del gioco
     * @return L'istanza di SudokuGameImpl caricata dal file
     * @throws IOException se si verifica un errore durante il caricamento del gioco
     * @throws ClassNotFoundException se la classe del gioco salvato non viene trovata
     */
    public static SudokuGameImpl CaricaPartita(String NomeFile) throws IOException, ClassNotFoundException {
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream(NomeFile))) {
            return (SudokuGameImpl) in.readObject();
        }
    }

}