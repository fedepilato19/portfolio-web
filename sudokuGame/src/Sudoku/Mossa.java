package Sudoku;

/**
 * Classe per memorizzare la singola mossa del giocatore 
 */
public class Mossa {
    int riga, col, precVal;

    Mossa(int riga, int col, int precVal) {
        this.riga = riga;
        this.col = col;
        this.precVal = precVal;
    }
}
