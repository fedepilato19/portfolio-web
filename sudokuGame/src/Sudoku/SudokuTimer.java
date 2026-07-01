package Sudoku;
import java.time.Duration;
import java.time.Instant;

/**
 * Una classe per misurare il tempo trascorso.
 */
public class SudokuTimer {
    private Instant tempoInizio;
    private Instant tempoFine;
    private Duration provTempotot;

    /**
     * Crea un nuovo timer con il tempo totale provvisorio (cioè quello prima di un eventuale salvatggio o dopo caricamento partita) inizializzato a zero.
     */
    public SudokuTimer() {
        provTempotot = Duration.ZERO;
    }

    /**
     * Imposta il tempo di inizio
     *
     * @param provTempotot Il tempo totale provvisorio da impostare
     */
    public void setTempoInizio(Duration provTempotot) {
        if (provTempotot != null) {
            this.provTempotot = provTempotot;
        } else {
            this.provTempotot = Duration.ZERO;
        }
    }

    /**
     * Avvia il timer.
     */

    public void start() {
        tempoInizio = Instant.now();
        tempoFine = null;  // Resetta il tempo finale quando inizia una nuova sessione di timer 
    }

    /**
     * Ferma il timer.
     */
    public void stop() {
        tempoFine = Instant.now();
    }

    /**
     * Restituisce il tempo trascorso totale (incluso il tempo impiegato in eventuali salvatggi precedenti)
     * formattato come una stringa di minuti e secondi.
     * @return Il tempo totale trascorso formattato come una stringa
     */
    public String Tempotot() {
        Instant fine;
        if (tempoFine != null) {
            fine = tempoFine;
        } else {
            fine = Instant.now();
        }
        Duration TempoTot = Duration.between(tempoInizio, fine).plus(provTempotot);
        long minuti = TempoTot.toMinutes();
        long secondi = TempoTot.getSeconds() % 60;
        return minuti + " minuti e " + secondi + " secondi";
    }

    /**
     * Restituisce il tempo trascorso quando effettuamo un salvatggio (e chiudiamo provvisoriamente la partita).
     *
     * @return Il tempo trascorso provvisorio
     */
    public Duration stampaProvTempotot() {
        Instant fine;
        if (tempoFine != null) {
            fine = tempoFine;
        } else {
            fine = Instant.now();
        }
        return Duration.between(tempoInizio, fine).plus(provTempotot);
    }
}