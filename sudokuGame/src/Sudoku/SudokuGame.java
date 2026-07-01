package Sudoku;
/**
 * Interfaccia per il gioco del Sudoku.
 */
public interface SudokuGame {
    /**
     * Avvia una nuova partita con il livello di difficoltà specificato.
     *
     * @param level Il livello di difficoltà (1, 2 o 3)
     */
    void startGame(int level);

    /**
     * Esegue una mossa del giocatore impostando un valore in una specifica cella della griglia.
     *
     * @param row L'indice della riga (0-8)
     * @param col L'indice della colonna (0-8)
     * @param value Il valore da inserire nella cella (1-9) oppure 0 per cancellare la cella
     */
    void makeMove(int row, int col, int value);

    /**
     * Verifica se la griglia di Sudoku è stata completata correttamente.
     *
     * @return true se il gioco è completo, false altrimenti
     */
    boolean isGameComplete();

    /**
     * Visualizza la griglia di Sudoku.
     */
    void displayGrid();
}