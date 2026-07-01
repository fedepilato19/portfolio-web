package Sudoku;
import java.io.Serial;
import java.io.Serializable;

/**
 * Classe che rappresenta la griglia del Sudoku.
 * Implementa Serializable per permettere la serializzazione della griglia.
 */

public class SudokuGrid implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    private final int[][] griglia;

    /**
     * Costruttore che inizializza una griglia Sudoku vuota (9x9).
     */

    public SudokuGrid() {
        griglia = new int[9][9];
    }

    /**
     * Verifica se gli indici forniti sono validi per la griglia Sudoku.
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @return true se gli indici sono validi, false altrimenti
     */

    public boolean Validita_Indici(int riga, int col) {
        return riga >= 0 && riga < 9 && col >= 0 && col < 9;
    }

    /**
     * Imposta il valore di una cella nella griglia Sudoku.
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @param val il valore da impostare nella cella
     */

    public void setCella(int riga, int col, int val) {
        if (Validita_Indici(riga, col)) {
            griglia[riga][col] = val;
        }
    }

    /**
     * Ottiene il valore di una cella nella griglia Sudoku.
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @return il valore della cella, o -1 se gli indici non sono validi
     */

    public int Cella(int riga, int col) {
        if (Validita_Indici(riga, col)) {
            return griglia[riga][col];
        } else {
            return -1; // o un altro valore che indichi un errore
        }
    }

    /**
     * Verifica se una cella nella griglia Sudoku è vuota.
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @return true se la cella è vuota, false altrimenti
     */

    public boolean CellaVuota(int riga, int col) {
        if (Validita_Indici(riga, col)) {
            return griglia[riga][col] == 0;
        } else {
            return false;
        }
    }

    /**
     * Verifica se una mossa è valida nella griglia Sudoku, in particolare dopo aver controllato la validità degli
     * indici, controlla se la cella nella quale vogliamo inserire il valore è vuota. Se tutto va a buon fine ritorna
     * PosizioneValida che controllo se quel valore è gia presente nella stessa sottogriglia, riga o colonna
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @param val il valore da inserire
     * @return true se la mossa è valida, false altrimenti
     */

    public boolean MossaValida(int riga, int col, int val) {
        if (Validita_Indici(riga, col)) {
            if (!CellaVuota(riga, col)) {
                return false;
            }
            return PosizioneValida(riga, col, val);
        } else {
            return false;
        }
    }

    /**
     * Verifica se una posizione è valida nella griglia Sudoku.
     * Controlla la riga, la colonna e la sottogriglia 3x3 per vedere se il valore è già presente.
     *
     * @param riga l'indice della riga
     * @param col l'indice della colonna
     * @param val il valore da inserire
     * @return true se la posizione è valida, false altrimenti
     */

    public boolean PosizioneValida(int riga, int col, int val) {
        for (int i = 0; i < 9; i++) {
            if (i != col && griglia[riga][i] == val) {
                return false;
            }
            if (i != riga && griglia[i][col] == val) {
                return false;
            }
        }
        int IndiceRigaInizio = (riga / 3) * 3;
        int IndiceColonnaInizio = (col / 3) * 3;
        for (int i = IndiceRigaInizio; i < IndiceRigaInizio + 3; i++) {
            for (int j = IndiceColonnaInizio; j < IndiceColonnaInizio + 3; j++) {
                if ((i != riga || j != col) && griglia[i][j] == val) {
                    return false;
                }
            }
        }
        return true;
    }

}