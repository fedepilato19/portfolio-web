const express = require('express');
const mysql = require('mysql2');
const bodyParser = require('body-parser');
const path = require('path');
const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.static(path.join(__dirname)));

// Connessione al database MySQL
const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'Database2024.',
    database: 'gym'
});

db.connect(err => {
    if (err) {
        console.error('Errore di connessione al database: ' + err.stack);
        return;
    }
    console.log('Connesso al database MySQL!');
});


/*GESTIONE RICHIESTE DELLA MIA API*/ 

//gestione login da parte del'utente
app.post('/login', (req, res) => {
    const { username, password } = req.body;

    db.query(
        'SELECT * FROM utenti WHERE username = ? AND password = ?',
        [username, password],
        (err, results) => {
            if (err) {
                console.error('Errore durante la query: ', err);
                return res.status(500).json({ success: false, message: 'Errore server' });
            }

            if (results.length === 0) {
                return res.json({ success: false, message: 'Credenziali errate' });
            }

            if (results[0].approved === 0) {
                return res.json({ success: false, message: 'Attendi l’approvazione dell’admin.' });
            }

            res.json({
                success: true,
                message: 'Login effettuato con successo!',
                username: results[0].username
            });
        }
    );
});

//Gestione della richiesta post per la registrazione di un nuovo utente
app.post('/register', (req, res) => {
    const { username, password, email } = req.body;

    db.query('SELECT * FROM utenti WHERE username = ?', [username], (err, results) => {
        if (err) {
            console.error('Errore durante la query: ', err);
            return res.status(500).json({ success: false, message: 'Errore server' });
        }

        if (results.length > 0) {
            return res.json({ success: false, message: 'Username già in uso' });
        }

        db.query('INSERT INTO utenti (username, password, approved, email) VALUES (?, ?, 0, ?)', 
            [username, password, email], (err, results) => {
            if (err) {
                console.error('Errore durante l’inserimento: ', err);
                return res.status(500).json({ success: false, message: 'Errore server' });
            }

            console.log(`Nuova richiesta di registrazione: ${username}`);

            res.json({ success: true, message: 'Registrazione in attesa di approvazione. Attendi la conferma.' });
        });
    });
});

//gestione dell'amministratore per le richieste di registrazione
app.get('/pending-users', (req, res) => {
    db.query('SELECT id, username, email FROM utenti WHERE approved = 0', (err, results) => {
        if (err) {
            console.error('Errore durante la query: ', err);
            return res.status(500).json({ success: false, message: 'Errore server' });
        }
        res.json(results);
    });
});
//gestione dell'amministratore per approvare le richieste da parte dei nuovi utenti
app.post('/admin/approveUser/:id', (req, res) => {
    const { id } = req.params;

    db.query('UPDATE utenti SET approved = 1 WHERE id = ?', [id], (err, results) => {
        if (err) {
            console.error('Errore durante l\'approvazione dell\'utente: ', err);
            return res.status(500).json({ success: false, message: 'Errore server' });
        }

        if (results.affectedRows === 0) {
            return res.json({ success: false, message: 'Utente non trovato' });
        }

        res.json({ success: true, message: 'Utente approvato con successo!' });
    });
});

//gestione dell'amministratore per eliminare le richieste da parte dei nuovi utenti

app.post('/admin/rejectUser/:id', (req, res) => {
    const { id } = req.params;

    db.query('DELETE FROM utenti WHERE id = ?', [id], (err) => {
        if (err) {
            console.error('Errore durante il rifiuto dell\'utente: ', err);
            return res.status(500).json({ success: false, message: 'Errore server' });
        }

        res.json({ success: true, message: 'Utente rifiutato!' });
    });
});

app.get('/getSavedExercises', (req, res) => {
    const query = 'SELECT * FROM esercizi';
    db.query(query, (err, result) => {
        if (err) {
            console.error('Errore durante il recupero degli esercizi:', err);
            return res.status(500).send('Errore del server');
        }
        res.json(result);
    });
});

app.post("/addExerciseToWorkout", (req, res) => {
    const { username, exercise, day, sets } = req.body;
    
    // Prima otteniamo l'ID dell'utente
    db.query('SELECT id FROM utenti WHERE username = ?', [username], (err, userResults) => {
        if (err) {
            console.error('Errore durante la query utente:', err);
            return res.status(500).json({ success: false, message: 'Errore nel server' });
        }
        
        if (userResults.length === 0) {
            return res.status(404).json({ success: false, message: 'Utente non trovato' });
        }

        const userId = userResults[0].id;

        // Ora otteniamo l'ID dell'esercizio
        db.query('SELECT id FROM esercizi WHERE nome = ?', [exercise], (err, exerciseResults) => {
            if (err) {
                console.error('Errore durante la query esercizio:', err);
                return res.status(500).json({ success: false, message: 'Errore nel server' });
            }

            if (exerciseResults.length === 0) {
                return res.status(404).json({ success: false, message: 'Esercizio non trovato' });
            }

            const exerciseId = exerciseResults[0].id;

            // Ora possiamo inserire i dati nella tabella piano_workout
            const sql = "INSERT INTO piano_workout (serie, giorno, esercizio_id, utente_id) VALUES (?, ?, ?, ?)";
            db.query(sql, [sets, day, exerciseId, userId], (err, result) => {
                if (err) {
                    console.error('Errore durante l\'inserimento nell\'esercizio:', err);
                    return res.status(500).json({ success: false, message: 'Errore nel salvataggio' });
                }
                res.json({ success: true });
            });
        });
    });
});

// Modifica la route per ottenere gli esercizi specifici dell'utente
app.get('/getUserWorkouts', (req, res) => {
    const username = req.query.username;
    
    db.query('SELECT id FROM utenti WHERE username = ?', [username], (err, results) => {
        if (err || results.length === 0) {
            return res.status(500).json({ success: false, message: 'Utente non trovato' });
        }

        const userId = results[0].id;
        
        const query = 'SELECT * FROM piano_workout WHERE user_id = ?';
        db.query(query, [userId], (err, workouts) => {
            if (err) {
                return res.status(500).json({ success: false, message: 'Errore nel recupero degli esercizi' });
            }
            res.json({ success: true, workouts: workouts });
        });
    });
});

// Svuota la scheda dell'utente
app.post("/clearUserWorkouts", (req, res) => {
    const { username } = req.body;
    
    db.query('SELECT id FROM utenti WHERE username = ?', [username], (err, results) => {
        if (err || results.length === 0) {
            return res.status(500).json({ success: false, message: 'Utente non trovato' });
        }

        const userId = results[0].id;

        // Elimina tutti gli esercizi dalla tabella piano_workout per quell'utente
        db.query('DELETE FROM piano_workout WHERE user_id = ?', [userId], (err) => {
            if (err) {
                return res.status(500).json({ success: false, message: 'Errore durante l\'eliminazione degli esercizi' });
            }
            res.json({ success: true, message: 'Scheda svuotata con successo' });
        });
    });
});

app.delete("/deleteAllExercises", async (req, res) => {
    try {
        db.query("DELETE FROM piano_workout");
    } catch (error) {
        console.error("Errore eliminazione esercizi:", error);
        res.status(500).json({ error: "Errore del server" });
    }
});

app.listen(port, () => {
    console.log(`Server in ascolto sulla porta ${port}`);
});

