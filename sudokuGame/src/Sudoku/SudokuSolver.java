package Sudoku;
import java.io.Serial;
import java.io.Serializable;
import java.util.Random;

/**
 * Questa classe fornisce metodi per risolvere puzzle di Sudoku.
 *
 * @version 1.0
 */
public class SudokuSolver implements Serializable {
    @Serial
    private static final long serialVersionUID = 1L;

    /** La griglia di Sudoku su cui operare. */
    private final SudokuGrid griglia;

    /** Generatore di numeri casuali per la generazione del puzzle. */
    private final Random random = new Random();

    /**
     * Crea un nuovo risolutore di Sudoku per una specifica griglia.
     *
     * @param griglia La griglia di Sudoku su cui operare
     */
    public SudokuSolver(SudokuGrid griglia) {
        this.griglia = griglia;
    }


    /**
     * Risolve il puzzle di Sudoku corrente.
     *
     * @return true se il puzzle è risolvibile, false altrimenti
     */
    public boolean Risoluzione(SudokuGrid griglia) {
        for (int riga = 0; riga < 9; riga++) {
            for (int col = 0; col < 9; col++) {
                if (griglia.CellaVuota(riga, col)) {
                    for (int num = 1; num <= 9; num++) {
                        if (griglia.MossaValida(riga, col, num)) {
                            griglia.setCella(riga, col, num);
                            if (Risoluzione(griglia)) {
                                return true;
                            }
                            griglia.setCella(riga, col, 0);
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Rimuove un numero specificato di celle dalla griglia.
     *
     * @param count Il numero di celle da rimuovere
     */

    public void RimuovereCelle(int count) {
        for (int i = 0; i < count; i++) {
            int riga, col;
            do {
                riga = random.nextInt(9);
                col = random.nextInt(9);
            } while (griglia.CellaVuota(riga, col));
            griglia.setCella(riga, col, 0);
        }
    }
}